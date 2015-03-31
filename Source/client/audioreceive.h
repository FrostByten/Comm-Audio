#ifndef AUDIO_H
#define AUDIO_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "../server/server/multicast.h"

class AudioThread;

#define RECV_SIZE 4096

#define BITS_PER_SAMPLE 16
#define CHANNELS 2
#define SAMPLES_PER_SECOND 44100

#define CIRC_BUFF_SIZE 40960

class AudioReceive
{
public:
    AudioReceive();
    // Circular Buffer
    char *circ_buff;

/*private slots:
    void paused();
    void played();*/
private:
    void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
    void init();
    AudioThread *audioThread;
    // Networking globals
    static char achMCAddr[1024];
    static BOOL fFlag;
    WSADATA stWSAData;
    struct ip_mreq stMreq;
    SOCKADDR_IN stLclAddr, stSrcAddr;

    // Audio globals
    HWAVEOUT wo;
    LPWAVEHDR whdr1, whdr2, whdr3;

};

#endif // AUDIO_H
