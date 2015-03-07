//#include <Windows.h> Because header guards are for chumps...
#include "multicast.h"
#include "session.h"

//Link vlc
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

//Link winsock
#pragma comment(lib, "ws2_32.lib")

void getFileList(char *path = "\0");
void openMulticastSocket();
void setupMulticast();
void setupListenSocket();
void print(char *m);
void cleanup(int ret);

const char *file_types[TYPES_LENGTH] = { "wav", "mp3", "ogg", "pcm", "aac", "flac", "m4a" };

WSADATA stWSAData;
char achMCAddr[1024] = MULTICAST_ADDR;
SOCKADDR_IN stLclAddr, stDstAddr;
SOCKET hMulticast_Socket, hListen_Socket;

std::vector<media> queue;
std::vector<user> clients;
std::vector<char *> files;

int main(int argc, char* argv[])
{
	WSAStartup(0x0202, &stWSAData);

	print("Initializing server...");
	getFileList();

	openMulticastSocket();
	setupMulticast();

	setupListenSocket();

	cleanup(0);
}

void lel()
{
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	/* Setup streaming to memory */
	char smem_options[256];
	sprintf(smem_options, "#transcode{acodec=s16l,samplerate=44100,ab=705600,channels=2}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld}",
		(long long int)(intptr_t)(void*)&postRender, (long long int)(intptr_t)(void*)&preRender);
	std::cout << std::endl << "Transcoding options: " << smem_options << std::endl << std::endl;
	const char* const vlc_args[] = { "-I", "dummy", "--verbose=2", "--sout", smem_options };

	/* Create a new vlc instance */
	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if (inst == NULL)
		exit(0);

	/* Open the media */
	//m = libvlc_media_new_path(inst, "C:\\Users\\Lewis\\Documents\\Comm-Audio\\Source\\server\\server\\dingdong.mp3");
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

	//Wait until end of media
	while (libvlc_media_player_is_playing(mp));

		//if (libvlc_media_player_get_position(mp) > 0.0f)
			//printf("\rSent: %%%.2f", (libvlc_media_player_get_position(mp) * 100));

	/* Cleanup */
	libvlc_media_player_release(mp);

	print("Server running, press enter to stop...");
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

	/* Open a TCP socket */
	if ((hListen_Socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		perror("Unable to create control socket");
		cleanup(1);
	}

	/* Setup the address and port */
	memset((char *)&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(CONTROL_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bind the TCP socket */
	if(bind(hListen_Socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		perror("Unable to bind control socket");
		cleanup(1);
	}

	/* Listen for new clients */
	listen(hListen_Socket, MAX_BACKLOG);
}

void getFileList(char *path)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	if (path[0] == '\0')
	{
		path = (char*)malloc(FILE_BUFF_LENGTH);
		GetCurrentDirectory(FILE_BUFF_LENGTH, path);
		std::cout << "Searching for files in: " << path << std::endl;
		strcat(path, "\\*");
	}

	hFind = FindFirstFile(path, &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
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
				char *place = strrchr(ffd.cFileName, '.') + 1;
				for (int i = 0; i < TYPES_LENGTH; ++i)
				{
					if (!strcmp(file_types[i], place))
					{
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

void print(char *m)
{
	std::cout << m << std::endl;
}

void cleanup(int ret)
{
	std::cin.get();

	for (int i = 0; i < files.size(); ++i)
		free(files[i]);

	closesocket(hListen_Socket);
	closesocket(hMulticast_Socket);
	WSACleanup();

	exit(ret);
}
