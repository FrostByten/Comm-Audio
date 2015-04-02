#include "chatnet.h"

ChatNet::ChatNet()
{

}


/******************************************************************************************
 * FUNCTION ChatNet::Resv
 *
 * DESIGNER: Marc Vouve
 *
 * PROGRAMMER: Marc Vouve
 *
 * DATE: FRIDAY MARCH 13th
 *
 * REVISIONS: MARCH 24rd - Refactored for users and moved out of main function
 *
 * PROTOTYPE: void recv_msg(pollfd ** ufds, user ** users, int *socket_count, int *socket_max)
 *                                user ** usr: the information associated with the fds
 *                                pollfd ** ufds: all the connected clients.
 *                                int * socket_count: the number of clients currently connected.
 *                                int * socket_max: the numer of clients that can be connected.
 *
 * RETURNS INT: total bytes read.
 *
 * NOTES: this function will multiplex messages as being from the listen port or the
 * message ports.
 * ***************************************************************************************/

int ChatNet::Resv(int fd, char * buff)
{
    return recv(fd, buff, ChatNet::BUFFLEN, 0);
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
 * REVISIONS: MARCH 24rd - Refactored for users and moved out of main function
 *
 * PROTOTYPE: addrinfo * ChatNet::GetHostInfo(const char * address, const char * port)
 *                                              address: ip address
 *                                              port: port
 *
 * RETURNS INT: total bytes read.
 *
 * NOTES: this function abstracts opening a port.
 * ***************************************************************************************/
int ChatNet::Connect(int sock, sockaddr * addr, socklen_t size )
{
    return connect(sock, addr, size );
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
 * REVISIONS: MARCH 24rd - Refactored for users and moved out of main function
 *
 * PROTOTYPE: addrinfo * ChatNet::GetHostInfo(const char * address, const char * port)
 *                                              address: ip address
 *                                              port: port
 *
 * RETURNS INT: total bytes read.
 *
 * NOTES: this function abstracts opening a port.
 * ***************************************************************************************/
addrinfo * ChatNet::GetHostInfo(const char * address, const char * port)
{
    addrinfo hints, * res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if(address == NULL)
    {
        hints.ai_flags = AI_PASSIVE;
    }
    if(getaddrinfo(address, port, &hints, &res) != 0)
    {
        return NULL;
    }

    return res;
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
 * REVISIONS: MARCH 24rd - Refactored for users and moved out of main function
 *
 * PROTOTYPE: addrinfo * ChatNet::GetHostInfo(const char * address, const char * port)
 *                                              address: ip address
 *                                              port: port
 *
 * RETURNS INT: total bytes read.
 *
 * NOTES: this function abstracts opening a port.
 * ***************************************************************************************/
void ChatNet::Send(int fd, char * msg)
{
    send(fd, msg, strlen(msg), 0);
}

ChatNet::~ChatNet()
{

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
 * REVISIONS: MARCH 24rd - Refactored for users and moved out of main function
 *
 * PROTOTYPE: addrinfo * ChatNet::GetHostInfo(const char * address, const char * port)
 *                                              address: ip address
 *                                              port: port
 *
 * RETURNS INT: total bytes read.
 *
 * NOTES: this function abstracts opening a port.
 * ***************************************************************************************/
void ChatNet::Close(int sock)
{
    close(sock);
}

