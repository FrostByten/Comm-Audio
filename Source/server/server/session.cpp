#include "multicast.h"
#include "session.h"

DWORD empty;
const std::string blank(PROG_BAR_WIDTH + strlen(PROG_STRING), ' ');
bool paused, skip;
libvlc_media_player_t *mp;

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
	libvlc_media_t *m;
	libvlc_event_manager_t *em;

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
			free(list->second->front().location);
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
		em = libvlc_media_player_event_manager(mp);
		libvlc_media_release(m);
		libvlc_media_player_play(mp);

		libvlc_event_attach(em, libvlc_MediaPlayerEncounteredError, media_error, NULL);

		//Wait for player to load the media
		while (!libvlc_media_player_is_playing(mp) && !skip);

		//Wait until end of media
		std::cout << std::endl;
		while (libvlc_media_player_is_playing(mp) || paused)
		{
			libvlc_media_player_set_pause(mp, paused?1:0);
			if (skip)
			{
				paused = false;
				blank_line();
				break;
			}
			printPercent(libvlc_media_player_get_position(mp));
		}

		libvlc_media_player_release(mp);
		std::cout << std::endl << (skip?"Skipped":"Finished") << " media, loading next" << std::endl;
		skip = false;
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
		printf("\r%s%s%s", paused?PAUSE_STRING:PROG_STRING, std::string(bars, '#').c_str(), std::string(PROG_BAR_WIDTH - bars, '-').c_str());
	}
}

void CALLBACK client_read(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	unsigned int i;
	for (i = 0; i <= clients.size(); ++i)
	{
		if (clients[i].wol.Internal == lpOverlapped->Internal)
			break;
	}

	if (i >= clients.size())
	{
		perror("\nData available, but client unknown...");
		return;
	}

	if (cbTransferred == 0) //Client disconnected
		return;

	clients[i].bytes_recvd = cbTransferred;
	clients[i].buffer.buf[cbTransferred] = '\0';

	handleRequest(i);

	if (WSARecv(clients[i].socket, &clients[i].buffer, 1, &clients[i].bytes_recvd, &empty, &clients[i].wol, client_read) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		perror("Error reading from client");
}

void handleRequest(int c)
{
	int data_length = *((int*)&clients[c].buffer.buf[1]);
	blank_line();
	printf("\nRequest from client[%s], command code: %d, received %d bytes\n", clients[c].name, (byte)clients[c].buffer.buf[0], clients[c].bytes_recvd);

	if (clients[c].bytes_recvd < 5 || (clients[c].bytes_recvd - 5) < data_length) //Packet too small, discard
	{
		printf("Corrupt packet, expecting %s%d bytes, only got %d! Discarding...\n", clients[c].bytes_recvd<5?"atleast ":"", data_length+5, (byte)clients[c].bytes_recvd);
		return;
	}

	if ((byte)clients[c].buffer.buf[0] > SET_NAME) //Max allowed command code
	{
		printf("Invalid command code: %d, discarding...\n", clients[c].buffer.buf[0]);
		return;
	}

	blank_line();
	printf("Request valid, command: %d, continuing...\n", clients[c].buffer.buf[0]);
	redraw_prog_bar = true;

	switch (clients[c].buffer.buf[0])
	{
		case PLAYBACK:
			handlePlayback(c);
			break;
		case SET_NAME:
			handleName(c);
			break;
		case FILE_SELECT:
			handleSelect(c);
			break;
		case USER_LIST:
			handleUserList(c);
			break;
		case MESSAGE:
			handleMessage(c);
			break;
		case MUTE:
			handleMute(c);
			break;
	}
}

void handlePlayback(int c)
{
	printf("\nPlayback command: ");
	if (*((int*)&clients[c].buffer.buf[1]) < 1)
	{
		printf("Not enough arguments\n");
		return;
	}

	switch (clients[c].buffer.buf[5])
	{
		case PLAY:
			printf("Play\n");
			libvlc_media_player_set_pause(mp, 0);
			redraw_prog_bar = true;
			paused = false;
			break;
		case PAUSE:
			printf("Pause\n");
			redraw_prog_bar = true;
			libvlc_media_player_set_pause(mp, 1);
			paused = true;
			break;
		case SKIP:
			printf("Skip\n");
			skip = true;
			blank_line();
			redraw_prog_bar = true;
			break;
	}
}

void handleName(int c)
{
	printf("\nName change command: %s changed name to ", clients[c].name);

	int size = *((int*)&clients[c].buffer.buf[1]);
	int fullsize = 6 + strlen(clients[c].name) + size;
	char *mes = (char*)malloc(fullsize);
	mes[0] = SET_NAME;
	memcpy(mes + 1, &fullsize, sizeof(int));
	memcpy(mes + 5, clients[c].name, strlen(clients[c].name));
	mes[strlen(clients[c].name) + 5] = '\0';
	memcpy(mes + strlen(clients[c].name) + 6, clients[c].buffer.buf + 5, size);
	sendMessageToAll(mes, fullsize, c);

	free(clients[c].name);
	clients[c].name = (char*)malloc(size+1);
	memcpy(clients[c].name, clients[c].buffer.buf + 5, size);
	clients[c].name[size] = '\0';
	printf("%s\n", clients[c].name);
}

void handleSelect(int c)
{
	int size = *((int*)&clients[c].buffer.buf[1]);

	printf("\nQueue add/File select command ");
	if (size < 2)
	{
		printf("Not enough arguments\n");
		return;
	}

	char *loc = (char*)malloc(size);
	memcpy(loc, clients[c].buffer.buf + 6, size-1);
	loc[size-1] = '\0';

	printf("type: %s, location: %s\n", clients[c].buffer.buf[5] ? "URL" : "File", loc);
	media m;
	m.type = clients[c].buffer.buf[5] ? TYPE_FILE:TYPE_URL;
	m.location = loc;

	queue.push_back(m);
}

void handleUserList(int c)
{
	printf("\nClient[%s] is requesting user list, sending...\n", clients[c].name);

	for (unsigned int i = 0; i < clients.size(); ++i)
		if (i != c)
		{
			int len = strlen(clients[i].name);
			char *mes = (char*)malloc(len + 6);
			mes[0] = SET_NAME;
			memcpy(mes + 1, &len, sizeof(int));
			mes[5] = EXIST;
			memcpy(mes + 5, clients[i].name, len);
			sendMessage(c, mes, len + 5);
			free(mes);
		}
}

void handleMessage(int c)
{
	int len = *((int*)&clients[c].buffer.buf[1]) + strlen(clients[c].name) + 2;
	char *mes = (char*)malloc(len + 5);
	mes[0] = MESSAGE;
	memcpy(mes + 1, &len, sizeof(int));
	memcpy(mes + 4, clients[c].name, strlen(clients[c].name));
	mes[strlen(clients[c].name) + 5] = ':';
	mes[strlen(clients[c].name) + 6] = ' ';
	memcpy(mes + strlen(clients[c].name) + 7, clients[c].buffer.buf + 5, *((int*)&clients[c].buffer.buf[1]));
	sendMessageToAll(mes, len, c);
	free(mes);
}

void handleMute(int c)
{
	checkAdmin(c);

	bool mute = clients[c].buffer.buf[5]==1?true:false;
	int tomute = findUser(clients[c].buffer.buf + 6);

	if (tomute != -1)
	{
		clients[tomute].muted = mute;
		sendMessageToAll(clients[c].buffer.buf, clients[c].bytes_recvd);
	}
}

void sendMessage(int c, char *mes, int len)
{
	send(clients[c].socket, mes, len, NULL);
}

void sendMessageToAll(char *mes, int len, int except)
{
	for (unsigned int i = 0; i < clients.size(); ++i)
		if (i != except)
			sendMessage(i, mes, len);
}

bool checkAdmin(int c)
{
	for (unsigned int i = 0; i < admins.size(); ++i)
		if (strcmp(admins[i].c_str(), inet_ntoa(clients[c].address->sin_addr)) == 0)
			return true;

	int len = strlen(UNAUTH_MESSAGE);
	char *mes = (char*)malloc(len + 5);
	mes[0] = MESSAGE;
	memcpy(mes + 1, &len, sizeof(len));
	sprintf(mes + 5, UNAUTH_MESSAGE);
	sendMessage(c, mes, len + 5);
	free(mes);
	return false;
}

int findUser(char *c)
{
	for (unsigned int i = 0; i < clients.size(); ++i)
	{
		if (strcmp(clients[i].name, c) == 0)
			return i;
	}

	return -1;
}

void inline blank_line()
{
	std::cout << '\r' << blank.c_str() << '\r';
}

void media_error(const struct libvlc_event_t* event, void *userData)
{
	blank_line();
	printf("Error occured while playing media, skipping...\n");
	skip = true;
}