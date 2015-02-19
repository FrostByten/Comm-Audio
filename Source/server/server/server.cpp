#include <Windows.h>
#include <vlc/vlc.h>

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

int main(int argc, char* argv[])
{
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	inst = libvlc_new(0, NULL);

	if (inst == NULL)
		exit(0);
	m = libvlc_media_new_path(inst, "dingdong.mp3");

	if (m == NULL)
		exit(0);
	mp = libvlc_media_player_new_from_media(m);

	libvlc_media_release(m);
	libvlc_media_player_play(mp);

	Sleep(500000);

	return 0;
}
