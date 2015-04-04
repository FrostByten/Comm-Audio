#include "controlthread.h"

ControlThread::ControlThread(Networking * net) : sock(net)
{
    //run();
}

ControlThread::~ControlThread()
{

}

void ControlThread::run()
{
    message * mes = new message;
    while(sock->recvMessage(mes) > 0)
    {

        emit message_found(mes);
        mes = new message;  // make a new message so that we don't break stuff.
    }
    emit server_disconnect();
}

