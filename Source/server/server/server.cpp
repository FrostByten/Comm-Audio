//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vlc/vlc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

#pragma comment(lib, "ws2_32.lib")

#define MULTICAST_ADDR "234.5.6.7"
#define MULTICAST_PORT 8910
#define PACKET_SIZE 512

void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size);
void postRender(void *p_audio_data, uint8_t *p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);

WSADATA stWSAData;
char achMCAddr[1024] = MULTICAST_ADDR;
struct ip_mreq stMreq;
SOCKADDR_IN stLclAddr, stDstAddr;
BOOL fFlag = true;
u_long lTTL = 2;
SOCKET hSocket;

int main(int argc, char* argv[])
{
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	WSAStartup(0x0202, &stWSAData);

	/* Open a UDP socket */
	hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET)
		exit(1);

	/* Bind the socket so we can join the group */
	stLclAddr.sin_family = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port = 0; //any port
	int nret = bind(hSocket, (struct sockaddr*)&stLclAddr, sizeof(stLclAddr));
	if (nret == SOCKET_ERROR)
		exit(1);

	/* Join the multicast group */
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nret = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq));
	if (nret == SOCKET_ERROR)
		exit(1);

	/* Set the TTL */
	nret = setsockopt(hSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&lTTL, sizeof(lTTL));
	if(nret == SOCKET_ERROR)
		exit(1);

	/* Disable loop-back */
	fFlag = false;
	nret = setsockopt(hSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&fFlag, sizeof(fFlag));
	if (nret == SOCKET_ERROR)
		exit(1);

	/* Assign the multicast address */
	stDstAddr.sin_family = AF_INET;
	stDstAddr.sin_addr.s_addr = inet_addr(achMCAddr);
	stDstAddr.sin_port = htons(MULTICAST_PORT);

	/* Setup streaming to memory */
	char smem_options[256];
	sprintf(smem_options, "#transcode{acodec=s16l}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld}",
		(long long int)(intptr_t)(void*)&postRender, (long long int)(intptr_t)(void*)&preRender);
	std::cout << std::endl << "Transcoding options: " << smem_options << std::endl << std::endl;
	const char* const vlc_args[] = { "-I", "dummy", "--verbose=2", "--sout", smem_options };

	/* Create a new vlc instance */
	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if (inst == NULL)
		exit(0);

	/* Open the media */
	m = libvlc_media_new_location(inst, "http://incompetech.com/music/royalty-free/mp3-royaltyfree/Who%20Likes%20to%20Party.mp3");
	if (m == NULL)
	{
		MessageBox(NULL, "Unable to load media.", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);
	libvlc_media_player_play(mp);

	//Wait for player to load the media
	while (!libvlc_media_player_is_playing(mp));
	std::cout << "Loaded media!" << std::endl;

	//Wait until end of media
	while (libvlc_media_player_is_playing(mp));
		//if (libvlc_media_player_get_position(mp) > 0.0f)
			//printf("\rSent: %%%.2f", (libvlc_media_player_get_position(mp) * 100));

	std::cout << std::endl << "Finished sending media... Exiting" << std::endl;

	/* Cleanup */
	libvlc_media_player_release(mp);
	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size)
{
	*pp_pcm_buffer = (uint8_t*)malloc(size);
	SecureZeroMemory(*pp_pcm_buffer, size);
}

void postRender(void *p_audio_data, uint8_t *p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts)
{
	static bool first = true;
	char *buff;
	unsigned int buffpos = 0;

	/* Setup the buffer initially */
	if (first)
	{
		std::cout << std::endl << "---------------------------" << std::endl << "Loaded stream..." << std::endl << "Channels: " << channels << std::endl << "Rate: " << rate << "kHz" << std::endl << "Bps: " << bits_per_sample << std::endl << "Packet size: " << PACKET_SIZE << std::endl << "Samples per packet: " << PACKET_SIZE / (bits_per_sample/8) << std::endl << "Chunk size: " << size << std::endl;
		printf("Media start pos: 0x%X\n", p_pcm_buffer);
		std::cout << "---------------------------" << std::endl;
	}

	unsigned int remaining = size;
	unsigned int pack;
	while (remaining > 0)
	{
		pack = remaining % PACKET_SIZE;
		pack = pack ? pack : PACKET_SIZE;
		buff = (char*)malloc(pack);
		memcpy(buff, p_pcm_buffer + buffpos, pack);
		sendto(hSocket, buff, pack, 0, (struct sockaddr*)&stDstAddr, sizeof(stDstAddr));
		remaining -= pack;
		buffpos += pack;
	}





	/* Send and reset if we have reached the buffer size */
	/*if ((buffpos + size) > PACKET_SIZE)
	{
		sendto(hSocket, buff, buffpos, 0, (struct sockaddr*)&stDstAddr, sizeof(stDstAddr));
		buff = (char*)malloc(PACKET_SIZE);
		buffpos = 0;
	}

	/* Add to the buffer and continue */
	//memcpy(buff + buffpos, p_pcm_buffer, size);
	//buffpos += size;

	free(p_pcm_buffer);

	first = false;
}
