#ifndef SESSION_H
#define SESSION_H

#include <vlc/vlc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define FILE 0
#define URL 1

#define FILE_BUFF_LENGTH 1024
#define TYPES_LENGTH 7

extern SOCKET hMulticast_Socket;
extern SOCKADDR_IN stDstAddr;

void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size);
void postRender(void *p_audio_data, uint8_t *p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);

typedef struct media
{
	bool type;
	char location[FILE_BUFF_LENGTH];
} media;

typedef struct user
{
	SOCKET *socket;
	char *name;
	struct sockaddr *address;
} user;

#endif
