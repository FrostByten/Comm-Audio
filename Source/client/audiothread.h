#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "../server/server/multicast.h"

class AudioReceive;

#define RECV_SIZE 4096

#define BITS_PER_SAMPLE 16
#define CHANNELS 2
#define SAMPLES_PER_SECOND 44100

#define CIRC_BUFF_SIZE 40960

class AudioThread : public QThread
{
public:
    AudioThread(AudioReceive *_audio, SOCKADDR_IN _stSrcAddr);
    void run(void *lpParam);

private:
    AudioReceive *audio;
    SOCKADDR_IN stSrcAddr;
};

#endif // AUDIOTHREAD_H
