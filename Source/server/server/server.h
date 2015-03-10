#ifndef SERVER_H
#define SERVER_H

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
DWORD WINAPI acceptRoutine(LPVOID lpArg);
void inline disable_cursor();
void inline blank_line();

const char *file_types[TYPES_LENGTH] = { "wav", "mp3", "ogg", "pcm", "aac", "flac", "m4a" };

#endif
