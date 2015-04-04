#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include "QThread"
#include "networking.h"


class ControlThread : public QThread
{
    Q_OBJECT
public:
    ControlThread(Networking * net);
    ~ControlThread();
private:
    void run();
    Networking * sock;
signals:
	void message_found(message * msg);
    void server_disconnect();

};

#endif // CONTROLTHREAD_H
