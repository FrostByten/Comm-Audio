#include "audiothread.h"
#include "audioreceive.h"

AudioThread::AudioThread(AudioReceive *_audio, SOCKADDR_IN stSrcAddr)
{
    audio = _audio;
    stSrcAddr = stSrcAddr;
}

void AudioThread::run(void *lpParam)
{
    SOCKET *hSocket = (SOCKET*)lpParam;
    char *message = (char*)malloc(RECV_SIZE);
    int addr_size = sizeof(struct sockaddr_in);
    int nret;
    unsigned int numpack = 0;
    unsigned int position = 0;

    for (;;)
    {
        /* Receive from the multicast UDP port */
        nret = recvfrom(*hSocket, message, RECV_SIZE, 0, (struct sockaddr*)&stSrcAddr, &addr_size);
        if (nret < 0)
        {
            std::cout << "Failed to receive!" << std::endl;
            continue;
        }
        numpack++;

        /* Copy the message to the circular buffer */
        int space = (CIRC_BUFF_SIZE - position);
        if (nret > space) //Wrap-around
        {
            memcpy(audio->circ_buff + position, message, space);
            position = 0;
            nret -= space;
            message += space;
        }
        memcpy(audio->circ_buff + position, message, nret);
        position += nret;

        printf("Played %d %d byte packets\r", numpack, nret);
    }

    free(message);
}
