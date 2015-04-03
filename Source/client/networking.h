#ifndef NETWORKING_H
#define NETWORKING_H

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>


class Networking
{
public:
    Networking(const char * address, const char * port);
    ~Networking();
    static int Networking::get_host_info(const char * address, const char * port, addrinfo ** addr_info);
    static SOCKET Networking::Connect(addrinfo * addr);
    WSADATA wsaData;
	bool getConnected();
	void sendMessage(char, int, const char*);
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

#endif // NETWORKING_H
