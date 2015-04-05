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

	sock = (Connect(addr_info));
	if(sock < 0)
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
	{
		shutdown(sock, SD_BOTH);
        closesocket(sock);
	}
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

bool Networking::getConnected()
{
	return connected;
}

/*
 *
 *
 * DATE: April 3rd
 *
 * REVISION: April 4nd - Changed param to message type / removed magic numbers.
 *
 * PROGRAMMER: Lewis Scott
 *             Marc Vouve - revision 1 april 4
 *
 * DESIGNER: Lewis Scott
 *           Marc Vouve - revision 1 april 4
 *
 * NOTES: I changed this to use a message struct because they're going to be passed around
 * a lot. It should also be noted that the message type changed the char and the int to use
 * stdint's int8_t and int32_t this should decouple the program from hardware, although
 * it probably is unnessesary.
 *
 */
void Networking::sendMessage(message * msg)
{
    char *buf = new char[msg->len + HEADERLEN];
    buf[0] = msg->type;
    memcpy(buf + TYPELEN, (void *)&msg->len, sizeof(int32_t));
    memcpy(buf + HEADERLEN, msg->data, msg->len);

	std::cerr << "Sending: [" << msg->type << "]" << std::endl;

    if(send(sock, buf, msg->len + HEADERLEN, 0) == SOCKET_ERROR)
		std::cerr << "Error sending: " << WSAGetLastError() << std::endl;

	free(buf);
}

/******************************************************************************************
 * FUNCTION ChatNet::recvMessage()
 *
 * DESIGNER: Marc Vouve
 *
 * PROGRAMMER: Marc Vouve
 *
 * DATE: FRIDAY MARCH 13th
 *
 * REVISIONS:
 *
 * PROTOTYPE: message Networking::recvMessage(message ** msg)
 *
 * Returns message recv'd.
 *
 * NOTES: This function will dynamicly resize the buffer for what it reads.
 *        Check for 0 len to see if socket has disconnected.
 * ***************************************************************************************/
int Networking::recvMessage(message * msg)
{
	char *buffer = (char*)malloc(5);
	memset(buffer, 0, 5);
	int tot = 0;

	while(tot < 5)
		tot += recv(sock, buffer + tot, 5 - tot, 0);

	msg->type = buffer[0];
	memcpy(&(msg->len), buffer+1, sizeof(int));
	msg->data = (char*)malloc(msg->len + 1);

	std::cerr << "Message length: " << msg->len << std::endl;

	tot = 0;
	while(tot < msg->len)
		tot += recv(sock, msg->data + tot, msg->len - tot, 0);

	msg->data[msg->len] = '\0';

	free(buffer);
	return tot + 5;
}
