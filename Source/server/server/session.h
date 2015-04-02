#ifndef SESSION_H
#define SESSION_H

#include <vlc/vlc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <limits>
#include <fstream>
#include "multicast.h"

#pragma warning (disable : 4996)
#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

//because microsoft thought it would be funny to pre-define these... I'm not laughing.
#undef max 
#undef min

#define FILE_BUFF_LENGTH 1024
#define MIC_BUFFER_SIZE 1024
#define TYPES_LENGTH 7
#define PROG_BAR_WIDTH 60
#define PROG_STRING "Progress: "
#define PAUSE_STRING "Paused:   "
#define UNAUTH_MESSAGE "You are no authorized to perform this function"
#define FILE_DENIED_MESSAGE "Access to this file is denied"
#define NO_OPEN_MESSAGE "This file cannot be opened for some reason..."

//#define DEBUG

#define STREAM_OPTIONS "#transcode{acodec=s16l,samplerate=44100,ab=705600,channels=2}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld}"

typedef std::numeric_limits<short> short_range;

extern SOCKET hMulticast_Socket;
extern SOCKADDR_IN stDstAddr;

extern bool redraw_prog_bar;
extern const char *file_types[TYPES_LENGTH];

void preRender(void*, uint8_t**, size_t);
void postRender(void*, uint8_t*, unsigned int, unsigned int, unsigned int, unsigned int, size_t, int64_t);

DWORD WINAPI mediaRoutine(LPVOID);
void CALLBACK client_read(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK mic_read(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void media_error(const struct libvlc_event_t* event, void *userData);
void sendMessage(int, char*, int);
void sendMessageToAll(char*, int , int = -1);
int findUser(char*);
bool checkAdmin(int);
void handleRequest(int);
void handlePlayback(int);
void handleName(int);
void handleSelect(int);
void handleUserList(int);
void handleMessage(int);
void handleMute(int);
void handleFileList(int);
void handleFileRequest(int);
void mixSamples(char*, char*, int);

void inline blank_line();
void inline printPercent(float through);

typedef enum media_type
{
	TYPE_FILE,
	TYPE_URL
} media_type;

typedef enum user_list
{
	EXIST, CONNECT, DISCONNECT
} user_list;

typedef struct media
{
	media_type type;
	char *location;
} media;

typedef struct user
{
	SOCKET socket;
	char *name;
	struct sockaddr_in *address;
	DWORD bytes_recvd;
	bool muted;
	WSABUF buffer;
	WSAOVERLAPPED wol;
} user;

extern WSABUF mic_buffer;
extern DWORD mic_bytes_recvd;
extern WSAOVERLAPPED mic_wol;
extern struct sockaddr_in *mic_from;
extern SOCKET hMicrophone_Socket;

extern std::vector<user> clients;
extern std::list<media> queue;
extern std::vector<std::string> admins;
extern std::vector<char *> files;

#endif
