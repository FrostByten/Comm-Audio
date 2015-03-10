#include "multicast.h"
#include "session.h"

void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size)
{
	*pp_pcm_buffer = (uint8_t*)malloc(size);
	SecureZeroMemory(*pp_pcm_buffer, size);
}

void postRender(void *p_audio_data, uint8_t *p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts)
{
	static bool first = true;
	char *buff;
	unsigned int buffpos = 0;

	/* Setup the buffer initially */
	if (first)
	{
#ifdef DEBUG
		std::cout << std::endl << "---------------------------" << std::endl << "Loaded stream..." << std::endl << "Channels: " << channels << std::endl << "Rate: " << rate << "Hz" << std::endl << "Bps: " << bits_per_sample << std::endl << "Packet size: " << PACKET_SIZE << std::endl << "Samples per packet: " << PACKET_SIZE / (bits_per_sample / 8) << std::endl << "Chunk size: " << size << std::endl;
		std::cout << "nb_samples: " << nb_samples << std::endl;
		printf("Media start pos: 0x%X\n", p_pcm_buffer);
		std::cout << "---------------------------" << std::endl;
#endif
	}

	unsigned int remaining = size;
	unsigned int pack;
	while (remaining > 0)
	{
		pack = remaining % PACKET_SIZE;
		pack = pack ? pack : PACKET_SIZE;
		buff = (char*)malloc(pack);
		memcpy(buff, p_pcm_buffer + buffpos, pack);
		sendto(hMulticast_Socket, buff, pack, 0, (struct sockaddr*)&stDstAddr, sizeof(stDstAddr));
		remaining -= pack;
		buffpos += pack;
		free(buff);
	}

	free(p_pcm_buffer);
	first = false;
}

DWORD WINAPI mediaRoutine(LPVOID lpArg)
{
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	std::pair<std::vector<char *>*, std::list<media>*> *list = (std::pair<std::vector<char *>*, std::list<media>*>*)lpArg;

	/* Setup streaming to memory */
	char smem_options[256];
	sprintf(smem_options, STREAM_OPTIONS, (long long int)(intptr_t)(void*)&postRender, (long long int)(intptr_t)(void*)&preRender);
#ifdef DEBUG
	std::cout << std::endl << "Transcoding options: " << smem_options << std::endl << std::endl;
#endif
	const char* const vlc_args[] = { "-I", "dummy", "--verbose=0", "--sout", smem_options };

	/* Create a new vlc instance */
	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
	if (inst == NULL)
		exit(0);

	for (;;)
	{
		//Load some media
		if (list->second->size() > 0)
		{

			if (list->second->front().type == TYPE_FILE)
				m = libvlc_media_new_path(inst, list->second->front().location);
			else
				m = libvlc_media_new_location(inst, list->second->front().location);

			//Pop the front element
			list->second->pop_front();
		}
		else
		{
			std::cout << "No media in queue, choosing random file" << std::endl;
			int choice = rand() % list->first->size();
			std::cout << "Using: " << list->first->at(choice) << std::endl;
			m = libvlc_media_new_path(inst, list->first->at(choice));
		}

		if (m == NULL)
		{
			std::cout << "Unable to load media, skipping..." << std::endl;
			continue;
		}

		//Load media into player
		mp = libvlc_media_player_new_from_media(m);
		libvlc_media_release(m);
		libvlc_media_player_play(mp);

		//Wait for player to load the media
		while (!libvlc_media_player_is_playing(mp));

		//Wait until end of media
		std::cout << std::endl;
		while (libvlc_media_player_is_playing(mp))
			printPercent(libvlc_media_player_get_position(mp));

		libvlc_media_player_release(mp);
		std::cout << std::endl << "Finished media, loading next" << std::endl;
	}

	return 0;
}

void inline printPercent(float through)
{
	static int bars = -1;

	if (bars != (int)(through * PROG_BAR_WIDTH) || redraw_prog_bar)
	{
		redraw_prog_bar = false;
		bars = through * PROG_BAR_WIDTH;
		printf("\rProgress: %s%s", std::string(bars, '#').c_str(), std::string(PROG_BAR_WIDTH - bars, '-').c_str());
	}
}
