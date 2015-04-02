#ifndef CHATNET_H
#define CHATNET_H

class ChatNet;

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "message.h"


class ChatNet
{
public:
    //Message Read();
    void Select();
    static int Connect(int, sockaddr *, socklen_t);
    static void Close(int sock);
    static addrinfo * GetHostInfo(const char *, const char *port);
    static int Resv(int fd, char * buff);
    static void Send(int fd, char * msg);
    ChatNet();
    ~ChatNet();

    static const int BUFFLEN = 255;
private:

    fd_set listenSet;


};

#endif // CHATNET_H
