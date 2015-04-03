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
private:
    SOCKET sock;
    bool connected;
};

#endif // NETWORKING_H
