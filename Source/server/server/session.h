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
#include "multicast.h"

#define FILE_BUFF_LENGTH 1024
#define TYPES_LENGTH 7
#define PROG_BAR_WIDTH 60
#define PROG_STRING "Progress: "
#define PAUSE_STRING "Paused:   "

//#define DEBUG

#define STREAM_OPTIONS "#transcode{acodec=s16l,samplerate=44100,ab=705600,channels=2}:smem{audio-postrender-callback=%lld,audio-prerender-callback=%lld}"

extern SOCKET hMulticast_Socket;
extern SOCKADDR_IN stDstAddr;

extern bool redraw_prog_bar;

void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size);
void postRender(void *p_audio_data, uint8_t *p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);

DWORD WINAPI mediaRoutine(LPVOID lpArg);
void CALLBACK client_read(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
void handleRequest(int);
void handlePlayback(int);
void handleName(int c);

void inline blank_line();
void inline printPercent(float through);

typedef enum media_type
{
	TYPE_FILE,
	TYPE_URL
} media_type;

typedef struct media
{
	media_type type;
	char location[FILE_BUFF_LENGTH];
} media;

typedef struct user
{
	SOCKET socket;
	char *name;
	struct sockaddr_in *address;
	DWORD bytes_recvd;
	WSABUF buffer;
	WSAOVERLAPPED wol;
} user;

extern std::vector<user> clients;

#endif
