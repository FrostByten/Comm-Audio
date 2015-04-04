#ifndef NETWORKING_H
#define NETWORKING_H

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdint.h>


struct message;

class Networking
{
public:
    Networking(const char * address, const char * port);
    ~Networking();
    static int Networking::get_host_info(const char * address, const char * port, addrinfo ** addr_info);
    static SOCKET Networking::Connect(addrinfo * addr);
    WSADATA wsaData;
	bool getConnected();
    void sendMessage(message * msg);
    int recvMessage(message * msg);
private:
    SOCKET sock;
    bool connected;
};

typedef enum command
{
	FILE_LIST, USER_LIST, MESSAGE, FILE_SELECT, MUTE, PLAYBACK, FILE_REQUEST, SET_NAME, CURRENT, FILE_EOF
} command;

typedef enum playback
{
	PLAY, PAUSE, SKIP
} playback;


struct message
{
    int8_t type;
    int32_t len;    // THIS SHOULD ONLY BE THE LENGTH OF THE DATA, ADD + 5 for header.
    char * data;
};

enum{BUFFERSIZE = 1024};
enum{HEADERLEN = 5, TYPELEN = 1};

#endif // NETWORKING_H
