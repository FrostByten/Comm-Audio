#include "controlthread.h"

ControlThread::ControlThread(Networking * net)
{
	sock = net;
    //run();
}

ControlThread::~ControlThread()
{

}

void ControlThread::run()
{
	std::cerr<<"Started control thread"<<std::endl;

    message * mes = new message;
    while(sock->recvMessage(mes) > 0)
    {
		std::cerr << "Received message successfully" << std::endl;
		emit message_found(mes);
        mes = new message;  // make a new message so that we don't break stuff.
    }
    emit server_disconnect();
	std::cerr << "Control thread dying..." << std::endl;
}
