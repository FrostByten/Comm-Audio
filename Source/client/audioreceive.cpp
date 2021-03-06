#include "audioreceive.h"
#include "audiothread.h"

char AudioReceive::achMCAddr[1024] = MULTICAST_ADDR;
BOOL AudioReceive::fFlag = true;

AudioReceive::AudioReceive()
{
    init();
}

void AudioReceive::init()
{
    WSAStartup(0x0202, &stWSAData);
    int nret;
    MMRESULT mmr;

    /* Allocate the circular buffer */
    circ_buff = (char*)malloc(CIRC_BUFF_SIZE);
    ZeroMemory(circ_buff, CIRC_BUFF_SIZE);

    /* Setup the desired waveout settings */
    WAVEFORMATEX wfx;
    wfx.nChannels = CHANNELS;
    wfx.nSamplesPerSec = SAMPLES_PER_SECOND;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.wBitsPerSample = BITS_PER_SAMPLE;
    wfx.cbSize = 0;
    wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.wBitsPerSample / 8;

    /* Open a waveout device */
    //mmr = waveOutOpen(&wo, WAVE_MAPPER, &wfx, (DWORD)WaveCallback, NULL, CALLBACK_FUNCTION);
    if (wo == NULL/* || mmr != MMSYSERR_NOERROR*/)
        std::cout << "Error: Unable to open waveout device!" << std::endl;
    else
        std::cout << "Successfully opened waveout device!" << std::endl;

    /* Prepare the wave header for the circular buffer */
    whdr1 = (LPWAVEHDR)malloc(sizeof(WAVEHDR));
    ZeroMemory(whdr1, sizeof(WAVEHDR));
    whdr1->lpData = circ_buff;
    whdr1->dwBufferLength = CIRC_BUFF_SIZE;
    whdr2 = (LPWAVEHDR)malloc(sizeof(WAVEHDR));
    whdr3 = (LPWAVEHDR)malloc(sizeof(WAVEHDR));
    memcpy(whdr2, whdr1, sizeof(WAVEHDR));
    memcpy(whdr3, whdr1, sizeof(WAVEHDR));

    //3 wave headers for triple buffering
    mmr = waveOutPrepareHeader(wo, whdr1, sizeof(WAVEHDR));
    mmr = waveOutPrepareHeader(wo, whdr2, sizeof(WAVEHDR));
    mmr = waveOutPrepareHeader(wo, whdr3, sizeof(WAVEHDR));
    if (mmr != MMSYSERR_NOERROR)
        std::cout << "Error preparing wave header, Error #:" << mmr << std::endl;

    /* Begin playing the circular buffer */
    mmr = waveOutWrite(wo, whdr1, sizeof(WAVEHDR));
    mmr = waveOutWrite(wo, whdr2, sizeof(WAVEHDR));
    mmr = waveOutWrite(wo, whdr3, sizeof(WAVEHDR));
    if (mmr != MMSYSERR_NOERROR)
        std::cout << "Error playing buffer, Error #: " << mmr << std::endl;

    /* Open a UDP socket */
    SOCKET hSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (hSocket == INVALID_SOCKET)
        exit(1);

    /* Set re-use for socket */
    fFlag = true;
    nret = setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&fFlag, sizeof(fFlag));
    if (nret == SOCKET_ERROR)
        exit(1);

    /* Bind the socket so we can join the group */
    stLclAddr.sin_family = AF_INET;
    stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stLclAddr.sin_port = htons(MULTICAST_PORT);
    nret = bind(hSocket, (struct sockaddr*)&stLclAddr, sizeof(stLclAddr));
    if (nret == SOCKET_ERROR)
        exit(1);

    /* Join the multicast group */
    stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    nret = setsockopt(hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq));
    if (nret == SOCKET_ERROR)
        exit(1);

    /* Create a thread to receive the messages */
    audioThread = new AudioThread(this, stSrcAddr);
    audioThread->run((void*)&hSocket);
    //hThread = CreateThread(NULL, 0, ThreadRoutine, &hSocket, 0, &ThreadId);
    /*if (hThread == NULL)
    {
        MessageBox(NULL, "Unable to create receive thread!", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }*/

    /* Wait for user input */
    std::cout << "Press any key to quit..." << std::endl << std::endl;
    std::cin.get();

    /* Drop the multicast membership */
    nret = setsockopt(hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&stMreq, sizeof(stMreq));
    if (nret == SOCKET_ERROR)
        printf("Error\tUnable to drop multicast membership!\n");
        //MessageBox(NULL, "Unable to drop multicast membership!", "Error", MB_OK | MB_ICONERROR);

    /* Cleanup */
    closesocket(hSocket);
    WSACleanup();
    waveOutUnprepareHeader(wo, whdr1, sizeof(WAVEHDR));
    waveOutUnprepareHeader(wo, whdr2, sizeof(WAVEHDR));
    waveOutUnprepareHeader(wo, whdr3, sizeof(WAVEHDR));
    waveOutClose(wo);
    free(whdr1);
    free(whdr2);
    free(whdr3);
    free(circ_buff);
}

void CALLBACK AudioReceive::WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    if (uMsg == WOM_DONE)
    {
        /* Signal bar graph thread with PCM */

        MMRESULT mmr = waveOutWrite(wo, (LPWAVEHDR)dw1, sizeof(WAVEHDR));
        if (mmr != MMSYSERR_NOERROR)
            std::cout << "Error playing buffer, Error #: " << mmr << std::endl;
    }
}

