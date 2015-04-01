#ifndef SERVER_H
#define SERVER_H

#pragma warning (disable : 4996)

//#include <Windows.h> Because header guards are for chumps...
#include "session.h"
#include "multicast.h"

//Link vlc
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

//Link winsock
#pragma comment(lib, "ws2_32.lib")

#define CLIENT_BUFFER_SIZE 1024

void getFileList(char *path = "\0");
void openMulticastSocket();
void setupMulticast();
void setupMicrophoneSocket();
void setupListenSocket();
void print(char *m);
void cleanup(int ret);
void loadAdmins();
DWORD WINAPI acceptRoutine(LPVOID lpArg);
void inline disable_cursor();

bool redraw_prog_bar = false;
const char *file_types[TYPES_LENGTH] = { "wav", "mp3", "ogg", "pcm", "aac", "flac", "m4a" };

#endif
