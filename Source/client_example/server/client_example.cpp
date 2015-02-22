//#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vlc/vlc.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

#pragma comment(lib, "ws2_32.lib")

#define MULTICAST_ADDR "234.5.6.7"
#define MULTICAST_PORT 8910

DWORD WINAPI ThreadRoutine(LPVOID lpParam);

WSADATA stWSAData;
char achMCAddr[1024] = MULTICAST_ADDR;
struct ip_mreq stMreq;
SOCKADDR_IN stLclAddr, stSrcAddr;
BOOL fFlag = true;

int main(int argc, char* argv[])
{
	HANDLE hThread;
	DWORD ThreadId;

	WSAStartup(0x0202, &stWSAData);
	int nret;

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
	std::cout << "Press any key to quit..." << std::endl;
	std::cin.get();

	/* Drop the multicast membership */
	nret = setsockopt(hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq));
	if (nret == SOCKET_ERROR)

	/* Cleanup */
	closesocket(hSocket);
	WSACleanup();

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
	char *message = (char*)malloc(1024);
	int addr_size = sizeof(struct sockaddr_in);
	int nret;

	for (;;)
	{
		nret = recvfrom(*hSocket, message, 1024, 0, (struct sockaddr*)&stSrcAddr, &addr_size);

		if (nret < 0)
			MessageBox(NULL, "Failed to receive!", "Error", MB_OK | MB_ICONERROR);
		else
			message[nret] = '\0';

		std::cout << message << std::endl;
	}
}
