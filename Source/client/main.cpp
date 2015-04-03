#include "mainwindow.h"
#include <QApplication>
#include <Windows.h>
// commenting out libvlc for testing purposes
#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

#pragma comment(lib, "Ws2_32.lib")

WSADATA wsaData;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);



    MainWindow w;
    w.show();

    WSACleanup();

    return a.exec();
}
