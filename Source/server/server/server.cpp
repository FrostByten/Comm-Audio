#include <Windows.h>
#include <vlc/vlc.h>

#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")

int main(int argc, char* argv[])
{
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	if (argc < 2)
	{
		MessageBox(NULL, "Need filename!", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	inst = libvlc_new(0, NULL);

	if (inst == NULL)
		exit(0);

	m = libvlc_media_new_path(inst, argv[1]);
	if (m == NULL)
	{
		MessageBox(NULL, "Unable to load media.", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);
	libvlc_media_player_play(mp);

	//Wait for open media
	Sleep(3000);

	/* Wait until the file is finished */
	libvlc_time_t time;
	while (libvlc_media_player_is_playing(mp))
	{
		/* Print elapsed time */
		time = libvlc_media_player_get_time(mp);
		printf("|---%d ms elapsed---|\r", time);
		Sleep(1);
	}
	OutputDebugString("finished?");
	libvlc_media_player_release(mp);

	return 0;
}
