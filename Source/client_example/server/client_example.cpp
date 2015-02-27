//#include <Windows.h> //Who needs header guards anyway?
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vlc/vlc.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#define MULTICAST_ADDR "234.5.6.7"
#define MULTICAST_PORT 8910
#define RECV_SIZE 4096

#define BITS_PER_SAMPLE 16
#define CHANNELS 2
#define SAMPLES_PER_SECOND 44100

#define CIRC_BUFF_SIZE 40960

DWORD WINAPI ThreadRoutine(LPVOID lpParam);

// Networking globals
WSADATA stWSAData;
char achMCAddr[1024] = MULTICAST_ADDR;
struct ip_mreq stMreq;
SOCKADDR_IN stLclAddr, stSrcAddr;
BOOL fFlag = true;

// Audio globals
HWAVEOUT wo;

// Circular Buffer
void *circ_buff;

int main(int argc, char* argv[])
{
	HANDLE hThread;
	DWORD ThreadId;

	WSAStartup(0x0202, &stWSAData);
	int nret;

	/* Allocate the circular buffer */
	circ_buff = malloc(CIRC_BUFF_SIZE);

	/* Setup the desired waveout settings */
	WAVEFORMATEX wfx;
	wfx.nChannels = CHANNELS;
	wfx.nSamplesPerSec = SAMPLES_PER_SECOND;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.wBitsPerSample = BITS_PER_SAMPLE;
	wfx.cbSize = 0;
	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

	/* Open a waveout device */
	MMRESULT mmr = waveOutOpen(&wo, WAVE_MAPPER, &wfx, NULL, NULL, CALLBACK_NULL);
	if (wo == NULL || mmr != MMSYSERR_NOERROR)
		std::cout << "Error: Unable to open waveout device!" << std::endl;
	else
		std::cout << "Successfully opened waveout device!" << std::endl;

	/* Open a UDP socket */
	SOCKET hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET)
		exit(1);

	/* Set re-use for socket */
	fFlag = true;
	nret = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&fFlag, sizeof(fFlag));
	if (nret == SOCKET_ERROR)
		exit(1);

	/* Bind the socket so we can join the group */
	stLclAddr.sin_family = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port = htons(MULTICAST_PORT);
	nret = bind(hSocket, (struct sockaddr*)&stLclAddr, sizeof(stLclAddr));
	if (nret == SOCKET_ERROR)
		exit(1);

	/* Join the multicast group */
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = INADDR_ANY;
	nret = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq));
	if (nret == SOCKET_ERROR)
		exit(1);

	/* Create a thread to receive the messages */
	hThread = CreateThread(NULL, 0, ThreadRoutine, &hSocket, 0, &ThreadId);
	if (hThread == NULL)
	{
		MessageBox(NULL, "Unable to create receive thread!", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	/* Wait for user input */
	std::cout << "Press any key to quit..." << std::endl << std::endl;
	std::cin.get();

	/* Drop the multicast membership */
	nret = setsockopt(hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq));
	if (nret == SOCKET_ERROR)
		MessageBox(NULL, "Unable to drop multicast membership!", "Error", MB_OK | MB_ICONERROR);

	/* Cleanup */
	closesocket(hSocket);
	WSACleanup();
	waveOutClose(wo);

	/*
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	inst = libvlc_new(0, NULL);

	if (inst == NULL)
		exit(0);
	
	m = libvlc_media_new_location(inst, message);
	if (m == NULL)
	{
		MessageBox(NULL, "Unable to load media.", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);
	libvlc_media_player_play(mp);

	//Wait for open media
	Sleep(3000);

	//Wait until the file is finished
	libvlc_time_t time;
	while (libvlc_media_player_is_playing(mp))
	{
		//Print elapsed time
		time = libvlc_media_player_get_time(mp);
		printf("|---%d ms elapsed---|\r", time);
		Sleep(1);
	}

	libvlc_media_player_release(mp);
	*/

	return 0;
}

DWORD WINAPI ThreadRoutine(LPVOID lpParam)
{
	SOCKET *hSocket = (SOCKET*)lpParam;
	char *message = (char*)malloc(RECV_SIZE);
	int addr_size = sizeof(struct sockaddr_in);
	int nret;
	unsigned int numpack = 0;

	for (;;)
	{
		/* Receive from the multicast UDP port */
		nret = recvfrom(*hSocket, message, RECV_SIZE, 0, (struct sockaddr*)&stSrcAddr, &addr_size);
		if (nret < 0)
		{
			std::cout << "Failed to receive!" << std::endl;
			continue;
		}
		numpack++;

		/* Setup the wave buffer header*/
		LPWAVEHDR whdr = (LPWAVEHDR)malloc(sizeof(WAVEHDR));
		ZeroMemory(whdr, sizeof(WAVEHDR));
		whdr->lpData = message;
		whdr->dwBufferLength = nret;

		MMRESULT mmr = waveOutPrepareHeader(wo, whdr, sizeof(WAVEHDR));
		if (mmr != MMSYSERR_NOERROR)
			std::cout << "Error preparing wave header, Error #:" << mmr << std::endl;

		/* Play the buffer */
		mmr = waveOutWrite(wo, whdr, sizeof(WAVEHDR));
		if (mmr != MMSYSERR_NOERROR)
			std::cout << "Error playing buffer, Error #: " << mmr << std::endl;
		else
			printf("Played %d %d byte packets\r", numpack, nret);
	}
}
