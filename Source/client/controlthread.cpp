#include "controlthread.h"


/*-------------------------------------------------------------------------
-- FUNCTION: ControlThread::ControlThread()
--
-- DATE: April 8, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: ControlThread::ControlThread()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This constructor takes a pointer to a networking and listens on it.
--      when something is read it spits it back.
-------------------------------------------------------------------------*/
ControlThread::ControlThread(Networking * net)
{
	sock = net;
    //run();
}

/*-------------------------------------------------------------------------
-- FUNCTION: ControlThread::~ControlThread()
--
-- DATE: April 8, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: ControlThread::~ControlThread()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--      The default destructor, doesn't do anything nothing to clean up.
-------------------------------------------------------------------------*/
ControlThread::~ControlThread()
{

}


/*-------------------------------------------------------------------------
-- FUNCTION: ControlThread::run()
--
-- DATE: April 8, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: ControlThread::run()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--      This function reads the control port for messages, and sends them
--      to the main file to be handled.
-------------------------------------------------------------------------*/
void ControlThread::run()
{
	std::cerr<<"Started control thread"<<std::endl;

    message * mes = new message;
    while(sock->recvMessage(mes) > 0)
    {
        std::cout << '\0';
		emit message_found(mes);
        mes = new message;  // make a new message so that we don't break stuff.
    }
    emit server_disconnect();
	std::cerr << "Control thread dying..." << std::endl;
}
