/*-------------------------------------------------------------------------
-- SOURCE FILE: server.h - Server header for Comm Audio
--
-- PROGRAM: Comm Audio Server
--
-- FUNCTIONS:
-- 		void getFileList(char* = "\0");
--		void openMulticastSocket();
--		void setupMulticast();
--		void setupMicrophoneSocket();
--		void setupListenSocket();
--		void print(char*);
--		void cleanup(int);
--		void loadAdmins();
--		DWORD WINAPI acceptRoutine(LPVOID);
--		void inline disable_cursor();
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- NOTES:
-------------------------------------------------------------------------*/

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
void openMicMulticastSocket();
void setupMicMulticast();
void setupMicrophoneSocket();
void setupListenSocket();
void print(char *m);
void cleanup(int ret);
void loadAdmins();
DWORD WINAPI acceptRoutine(LPVOID lpArg);
void inline disable_cursor();

bool redraw_prog_bar = false;
float seekby;
const char *file_types[TYPES_LENGTH] = { "wav", "mp3", "ogg", "pcm", "aac", "flac", "m4a" };

extern libvlc_media_player_t *mp;

#endif
