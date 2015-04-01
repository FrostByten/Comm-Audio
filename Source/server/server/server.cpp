#include "server.h"

WSADATA stWSAData;
char achMCAddr[MAX_ADDR_SIZE] = MULTICAST_ADDR;
SOCKADDR_IN stLclAddr, stDstAddr;
SOCKET hMulticast_Socket, hListen_Socket;
HANDLE hControl_Thread = INVALID_HANDLE_VALUE;
HANDLE hMedia_Thread = INVALID_HANDLE_VALUE;

std::list<media> queue;
std::vector<user> clients;
std::vector<char *> files;
std::vector<std::string> admins;

WSAEVENT event_accept, event_close;
DWORD read_flags = 0;

int main(int argc, char* argv[])
{
	WSAStartup(0x0202, &stWSAData);

	disable_cursor();

	print("Initializing server...\n");
	getFileList();
	print("Finished searching\n");

	if (files.empty())
	{
		printf("\nNo files available, exiting...");
		cleanup(0);
	}

	printf("Reading admins...\n");
	loadAdmins();
	printf("\nFinished reading admins\n");

	openMulticastSocket();
	setupMulticast();

	setupListenSocket();

	std::pair<std::vector<char *>*, std::list<media>*> list(&files, &queue);

	/* Create a thread to play media */
	DWORD thread_id;
	if ((hMedia_Thread = CreateThread(NULL, 0, mediaRoutine, &list, 0, &thread_id)) == NULL)
	{
		perror("Unable to create media thread");
		cleanup(1);
	}
	print("Media thread ready");

	print("\nServer running, press <Enter> to terminate\n");
	cleanup(0);
}

void setupMulticast()
{
	u_long lTTL = 2;
	BOOL fFlag = true;
	struct ip_mreq stMreq;

	/* Join the multicast group */
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = INADDR_ANY;

	if (setsockopt(hMulticast_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq)) == SOCKET_ERROR)
	{
		perror("Unable to join multicast group");
		cleanup(1);
	}

	/* Set the TTL */
	if (setsockopt(hMulticast_Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&lTTL, sizeof(lTTL)) == SOCKET_ERROR)
	{
		perror("Unable to set TTL for multicast");
		cleanup(1);
	}

	/* Disable loop-back */
	fFlag = false;
	if (setsockopt(hMulticast_Socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&fFlag, sizeof(fFlag)) == SOCKET_ERROR)
	{
		perror("Unable to disable loop-back for multicast");
		cleanup(1);
	}

	/* Assign the multicast address */
	stDstAddr.sin_family = AF_INET;
	stDstAddr.sin_addr.s_addr = inet_addr(achMCAddr);
	stDstAddr.sin_port = htons(MULTICAST_PORT);

	print("Multicast ready");
}

void openMulticastSocket()
{
	/* Open a UDP socket */
	if ((hMulticast_Socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		perror("Unable to create multicast socket");
		cleanup(1);
	}

	/* Bind the socket so we can join the group */
	stLclAddr.sin_family = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port = 0; //any port
	if (bind(hMulticast_Socket, (struct sockaddr*)&stLclAddr, sizeof(stLclAddr)) == SOCKET_ERROR)
	{
		perror("Unable to bind multicast socket");
		cleanup(1);
	}
}

void setupListenSocket()
{
	struct sockaddr_in server;
	u_long mode = 1;

	/* Open a TCP socket */
	if ((hListen_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		perror("Unable to create control socket");
		cleanup(1);
	}

	/* Set socket to non-blocking */
	if (ioctlsocket(hListen_Socket, FIONBIO, &mode) != NO_ERROR)
	{
		perror("Unable to set control socket to non-blocking mode");
		cleanup(1);
	}

	/* Setup the address and port */
	memset((char *)&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(CONTROL_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bind the TCP socket */
	if (bind(hListen_Socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		perror("Unable to bind control socket");
		cleanup(1);
	}

	/* Listen for new clients */
	listen(hListen_Socket, MAX_BACKLOG);

	/* Accept new clients */
	struct sockaddr *addr = (struct sockaddr*)malloc(sizeof(struct sockaddr));
	int addrlen = sizeof(struct sockaddr);

	/* Create accept event */
	if ((event_accept = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		perror("Unable to create accept event");
		cleanup(1);
	}

	/* Create close event */
	if ((event_close = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		perror("Unable to create close event");
		cleanup(1);
	}

	/* Post to the event when triggered */
	if (WSAEventSelect(hListen_Socket, event_accept, FD_ACCEPT))
	{
		perror("Unable to listen for clients, event failed");
		cleanup(1);
	}

	/* Create a thread to accept connection */
	DWORD thread_id;
	if ((hControl_Thread = CreateThread(NULL, 0, acceptRoutine, NULL, 0, &thread_id)) == NULL)
	{
		perror("Unable to create accept thread");
		cleanup(1);
	}

	print("Control connection ready");
}

void getFileList(char *path)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	/* This is for the first iteration */
	if (path[0] == '\0')
	{
		path = (char*)malloc(FILE_BUFF_LENGTH);
		GetCurrentDirectory(FILE_BUFF_LENGTH, path);
		std::cout << "Searching for files in: " << path << std::endl;
		strcat(path, "\\*");
	}

	/* Traverse the current directory */
	hFind = FindFirstFile(path, &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			/* Call recersively on directories */
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && ffd.cFileName[0] != '.')
			{
				char *subpath = (char*)malloc(FILE_BUFF_LENGTH);
				strcpy(subpath, path);
				subpath[strlen(path) - 1] = '\0';
				strcat(subpath, ffd.cFileName);
				strcat(subpath, "\\*");
				getFileList(subpath);
			}
			else
			{
				/* Check file-type against valid types */
				char *place = strrchr(ffd.cFileName, '.') + 1;
				for (int i = 0; i < TYPES_LENGTH; ++i)
				{
					if (!strcmp(file_types[i], place))
					{
						/* Note file in file list */
						std::cout << "\tFound: " << ffd.cFileName << std::endl;

						char *filepath = (char*)malloc(FILE_BUFF_LENGTH);
						strcpy(filepath, path);
						filepath[strlen(filepath) - 1] = '\0';
						strcat(filepath, ffd.cFileName);

						files.push_back(filepath);
						break;
					}
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	FindClose(hFind);
	free(path);
}

DWORD WINAPI acceptRoutine(LPVOID lpArg)
{
	HANDLE events[2] = { event_accept, event_close };

	for (;;)
	{
		/* Wait for an event or IOCR to be queued */
		//The Ex in WaitForMultipleObjectsEx allows the thread to be awoken by queued IOCP requests
		if (WaitForMultipleObjectsEx(2, events, FALSE, INFINITE, TRUE) - WAIT_OBJECT_0 == 0) //Accept event (index 0)
		{
			int client_len = sizeof(sockaddr_in);
			sockaddr_in *client_addr = (sockaddr_in*)malloc(client_len);

			/* Accept the connection */
			SOCKET client_sock = accept(hListen_Socket, (sockaddr*)client_addr, &client_len);
			blank_line();
			std::cout << "\r\tClient[" << inet_ntoa(client_addr->sin_addr) << "] connected" << std::endl;
			redraw_prog_bar = true;

			/* Add the client to the listen group for disconnecting */
			if (WSAEventSelect(client_sock, event_close, FD_CLOSE))
			{
				perror("Unable to listen for disconnection from this client, event failed");
				cleanup(1);
			}

			/* Construct the client struct*/
			user client;
			client.socket = client_sock;
			client.address = client_addr;
			client.name = (char*)malloc(strlen(inet_ntoa(client.address->sin_addr))+1);
			memcpy(client.name, inet_ntoa(client.address->sin_addr), strlen(inet_ntoa(client.address->sin_addr)));
			client.name[strlen(inet_ntoa(client.address->sin_addr))] = '\0';
			client.bytes_recvd = 0;
			client.buffer.buf = (char*)malloc(CLIENT_BUFFER_SIZE);
			client.buffer.len = CLIENT_BUFFER_SIZE;
			ZeroMemory(&client.wol, sizeof(WSAOVERLAPPED));

			int len = strlen(client.name);
			char *mes = (char*)malloc(len + 6);
			mes[0] = SET_NAME;
			memcpy(mes + 1, &len, sizeof(int));
			mes[5] = CONNECT;
			memcpy(mes + 5, client.name, len);
			sendMessageToAll(mes, len + 5);
			free(mes);

			/* Note the client in the client list */
			clients.push_back(client);

			/* Setup the socket to receive */
			if (WSARecv(client.socket, &client.buffer, 1, &client.bytes_recvd, &read_flags, &client.wol, client_read) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
				perror("Error reading from client");

			/* Reset to event ready to be triggered again */
			WSAResetEvent(event_accept);
		}
		else //Close event (index 1)
		{
			WSANETWORKEVENTS events;
			/* Search through clients to see which one disconnected */
			for (unsigned int i = 0; i < clients.size(); ++i)
			{
				/* Check for disconnection */
				WSAEnumNetworkEvents(clients[i].socket, NULL, &events);
				if (events.lNetworkEvents == FD_CLOSE)
				{
					blank_line();
					std::cout << "\r\tClient[" << clients[i].name << "] disconnected" << std::endl;
					redraw_prog_bar = true;

					int len = strlen(clients[i].name);
					char *mes = (char*)malloc(len + 6);
					mes[0] = SET_NAME;
					memcpy(mes + 1, &len, sizeof(int));
					mes[5] = DISCONNECT;
					memcpy(mes + 5, clients[i].name, len);
					sendMessageToAll(mes, len + 5);
					free(mes);

					/* Remove the client from the list and de-allocate */
					free(clients[i].address);
					free(clients[i].name);
					closesocket(clients[i].socket);
					free(clients[i].buffer.buf);
					clients.erase(clients.begin() + (i * sizeof(user)));
					break;
				}
			}
			
			/* Reset to event ready to be triggered again */
			WSAResetEvent(event_close);
		}
	}

	return 0;
}

void print(char *m)
{
	std::cout << m << std::endl;
}

void cleanup(int ret)
{
	std::cin.get();
	print("\nServer closing...");

	/* Kill the accept thread */
	if (hControl_Thread != INVALID_HANDLE_VALUE)
		TerminateThread(hControl_Thread, 0);

	/* Kill the media thread */
	if (hMedia_Thread != INVALID_HANDLE_VALUE)
		TerminateThread(hMedia_Thread, 0);

	/* De-allocate memory and disconnect from clients before we die */
	for (unsigned int i = 0; i < files.size(); ++i)
		free(files[i]);

	while (!queue.empty())
	{
		free(queue.front().location);
		queue.pop_front();
	}

	for (unsigned int i = 0; i < clients.size(); ++i)
	{
		free(clients[i].address);
		free(clients[i].name);
		shutdown(clients[i].socket, SD_BOTH);
		closesocket(clients[i].socket);
		free(clients[i].buffer.buf);
	}

	/* Network cleanup */
	WSACloseEvent(event_accept);
	WSACloseEvent(event_close);
	shutdown(hListen_Socket, SD_BOTH);
	shutdown(hMulticast_Socket, SD_BOTH);
	closesocket(hListen_Socket);
	closesocket(hMulticast_Socket);
	WSACleanup();

	exit(ret);
}

void loadAdmins()
{
	std::ifstream in_stream("admin.cfg");
	std::string line;

	while (getline(in_stream, line))
		admins.push_back(line);

	for (unsigned int i = 0; i < admins.size(); ++i)
		printf("\tLoaded admin: %s\n", admins[i].c_str());
}

void inline disable_cursor()
{
	CONSOLE_CURSOR_INFO info;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	info.bVisible = false;
	info.dwSize = 1;

	SetConsoleCursorInfo(hOutput, &info);
}
