#include "mainwindow.h"
#include <QApplication>
#include <Windows.h>
// commenting out libvlc for testing purposes
//#include <libvlc.h>
//#include <vlc/vlc.h>

//#pragma comment(lib, "libvlc.lib")
//#pragma comment(lib, "libvlccore.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    /*
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	inst = libvlc_new(0, NULL);

	if (inst == NULL)
		exit(0);
	m = libvlc_media_new_location(inst, "http://incompetech.com/music/royalty-free/mp3-royaltyfree/Who%20Likes%20to%20Party.mp3");

	if (m == NULL)
		exit(0);
	mp = libvlc_media_player_new_from_media(m);

	libvlc_media_release(m);
    libvlc_media_player_play(mp);*/

    return a.exec();
}
