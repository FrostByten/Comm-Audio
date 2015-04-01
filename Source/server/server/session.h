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
#include <fstream>
#include "multicast.h"

#pragma warning (disable : 4996)
#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

#define FILE_BUFF_LENGTH 1024
#define TYPES_LENGTH 7
#define PROG_BAR_WIDTH 60
#define PROG_STRING "Progress: "
#define PAUSE_STRING "Paused:   "
#define UNAUTH_MESSAGE "You are no authorized to perform this function"

//#define DEBUG

#define STREAM_OPTIONS "#transcode{acodec=s16l,samplerate=44100,ab=705600,channels=2}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld}"

extern SOCKET hMulticast_Socket;
extern SOCKADDR_IN stDstAddr;

extern bool redraw_prog_bar;

void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size);
void postRender(void *p_audio_data, uint8_t *p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);

DWORD WINAPI mediaRoutine(LPVOID lpArg);
void CALLBACK client_read(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
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

extern std::vector<user> clients;
extern std::list<media> queue;
extern std::vector<std::string> admins;

#endif
