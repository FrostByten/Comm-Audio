#include "networking.h"

#include <string.h>
#include <stdlib.h>




Networking::Networking(const char * address, const char * port)
{
    addrinfo * addr_info = nullptr;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
    }


    if(get_host_info(address, port, &addr_info) < 0)
    {
        connected = false;
        sock = -1;
        addr_info = nullptr;

        std::cout << "Could not get host info" << std::endl;
    }
    else if(sock = (Connect(addr_info)) < 0)
    {
        connected = false;
        sock = -1;

        std::cout << "Could not connect" << std::endl;
    }

    if(!addr_info)
    {
        freeaddrinfo(addr_info);
    }
}

Networking::~Networking()
{
    if(sock != -1)
        closesocket(sock);
    WSACleanup();

}

/******************************************************************************************
 * FUNCTION Networking::GetHostInfo
 *
 * DESIGNER: Marc Vouve
 *
 * PROGRAMMER: Marc Vouve
 *
 * DATE: FRIDAY MARCH 13th
 *
 * REVISIONS: MARCH 24rd - Refactored for users and moved out of main function
 *            APRIL 1st  - Migrated to Comm Audio, changed return to int, added addrinfo
 *                         as function parameter.
 *
 *
 * PROTOTYPE: addrinfo * ChatNet::GetHostInfo(const char * address, const char * port, addrinfo ** addr_info)
 *                                              address: ip address
 *                                              port: port
 *                                              addr_info: the address info struct.
 *
 * RETURNS -1 on failure.
 *
 * NOTES: This function looks gets the port addr info by a string of the port and the addr.
 *          if the address is null it assumes that it is a listening addr.
 * ***************************************************************************************/
int Networking::get_host_info(const char * address, const char * port, addrinfo ** addr_info)
{
    addrinfo hints, * res;
    int err;

    std::cout << "Addr: " << address << " port: " << port << std::endl;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if(address == NULL)
    {
        hints.ai_flags = AI_PASSIVE;    //
    }
    if((err = getaddrinfo(address, port, &hints, addr_info)) != 0)
    {
        std::cerr << err << std::endl;
        return -1;
    }
    else
        return 0;
}

/******************************************************************************************
 * FUNCTION ChatNet::Connect
 *
 * DESIGNER: Marc Vouve
 *
 * PROGRAMMER: Marc Vouve
 *
 * DATE: FRIDAY MARCH 13th
 *
 * REVISIONS: April 1st 2015 - Refactored for comm audio
 *
 * PROTOTYPE: ChatNet::Connect(SOCKET sock, addrinfo addr)
 *
 * RETURNS INT: total bytes read.
 *
 * NOTES: This function opens a port based off an addrinfo struct.
 * ***************************************************************************************/
SOCKET Networking::Connect(addrinfo * addr)
{
    SOCKET sockfd;
    addrinfo * p;

    std::cout << addr << std::endl;

    for(p = addr; p != nullptr; p = p->ai_next)
    {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            continue;
        }
        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            closesocket(sockfd);
            continue;
        }

        break;
    }

    if(p == NULL)
        return -1;
    else
        return sockfd;
}

