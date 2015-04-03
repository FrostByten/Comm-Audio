/*-------------------------------------------------------------------------
-- SOURCE FILE: session.cpp - Session for Comm Audio
--
-- PROGRAM: Comm Audio Server
--
-- FUNCTIONS:
-- 		void preRender(void*, uint8_t**, size_t);
-- 		void postRender(void*, uint8_t*, unsigned int, unsigned int, unsigned int, unsigned int, size_t, int64_t);
--
-- 		DWORD WINAPI mediaRoutine(LPVOID);
-- 		void CALLBACK client_read(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
-- 		void CALLBACK mic_read(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
-- 		void media_error(const struct libvlc_event_t* event, void *userData);
-- 		void sendMessage(int, char*, int);
-- 		void sendMessageToAll(char*, int , int = -1);
-- 		int findUser(char*);
-- 		bool checkAdmin(int);
-- 		void handleRequest(int);
-- 		void handlePlayback(int);
-- 		void handleName(int);
-- 		void handleSelect(int);
-- 		void handleUserList(int);
-- 		void handleMessage(int);
-- 		void handleMute(int);
-- 		void handleFileList(int);
-- 		void handleFileRequest(int);
-- 		void mixSamples(char*, char*, int);
--
-- 		void inline blank_line();
-- 		void inline printPercent(float through);
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- NOTES:
-------------------------------------------------------------------------*/

#include "multicast.h"
#include "session.h"

DWORD empty;
const std::string blank(PROG_BAR_WIDTH + strlen(PROG_STRING), ' ');
bool paused, skip;
libvlc_media_player_t *mp;

/*-------------------------------------------------------------------------
-- FUNCTION: preRender
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size);
--
-- PARAMETERS:
--		void *p_audio_data:			Pointer to the information about the data.
--		uint8_t **pp_pcm_buffer:	Pointer to place the memory block into.
--		size_t size:				The amount of data to allocate.
--
-- RETURNS: void
--
-- NOTES:
-- Allocates memory blocks for libvlc to render PCM into.
-------------------------------------------------------------------------*/
void preRender(void *p_audio_data, uint8_t **pp_pcm_buffer, size_t size)
{
	*pp_pcm_buffer = (uint8_t*)malloc(size);
	SecureZeroMemory(*pp_pcm_buffer, size);
}

/*-------------------------------------------------------------------------
-- FUNCTION: postRender
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void postRender(void *p_audio_data, uint8_t *p_pcm_buffer, unsigned int channels, unsigned int rate, unsigned int nb_samples, unsigned int bits_per_sample, size_t size, int64_t pts);
--
-- PARAMETERS:
--		void *p_audio_data:				Information about the data.
--		uint8_t *p_pcm_buffer:			Pointer to the memory block where the data was rendered.
--		unsigned int channels:			The number of audio channels.
--		unsigned int rate:				The sample rate, in Hz.
--		unsigned int nb_samples:		The number of rendered samples.
--		unsigned int bits_per_sample:	The number of bits per audio sample.
--		size_t size:					The size of the rendered data in bytes.
--		int64_t pts:					The number of audio keyframes.
--
-- RETURNS: void.
--
-- NOTES:
-- Prepares the rendered PCM audio into chunks and sends them via multicast UDP.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: mediaRoutine
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: DWORD WINAPI mediaRoutine(LPVOID lpArg);
--
-- PARAMETERS:
--		LPVOID lpArg: Pointer to the startup arguments for the thread.
--
-- RETURNS: DWORD: The return value for the thread.
--
-- NOTES:
-- Starting point for the media control thread. Renders audio and sends it via
-- the multicast UDP socket.
-------------------------------------------------------------------------*/
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

		char *message = (char*)malloc(FILE_BUFF_LENGTH);
		sprintf(message, "Now playing: %s%s%s", libvlc_media_get_meta(m, libvlc_meta_Title), libvlc_media_get_meta(m, libvlc_meta_Artist) ? " - " : "", libvlc_media_get_meta(m, libvlc_meta_Artist) ? libvlc_media_get_meta(m, libvlc_meta_Artist) : "");
		printf("\n%s", message);
		int len = strlen(message + strlen("Now playing: "));
		char *msg = (char*)malloc(len + 5);
		msg[0] = CURRENT;
		memcpy(msg + 1, &len, sizeof(int));
		memcpy(msg + 5, message + strlen("Now playing: "), len);
		sendMessageToAll(msg, len + 5);
		free(msg);
		free(message);

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

/*-------------------------------------------------------------------------
-- FUNCTION: printPercent
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void inline printPercent(float through);
--
-- PARAMETERS:
--		float through: 0-1, percentage of the bar to fill.
--
-- RETURNS: void.
--
-- NOTES:
-- Prints a line to the console that represents how far through the song the
-- media thread currently is.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: client_read
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void CALLBACK client_read(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
--
-- PARAMETERS:
--		DWORD dwError:					The error, if any occurred.
--		DWORD cbTransferred:			The number of bytes transferred.
--		LPWSAOVERLAPPED lpOverlapped:	A pointer to the overlapped structure.
--		DWORD dwFlags:					The flags used during reading.
--
-- RETURNS: void.
--
-- NOTES:
-- Reads from a client.
-------------------------------------------------------------------------*/
void CALLBACK client_read(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	unsigned int i;
	for (i = 0; i < clients.size(); ++i)
	{
		if (clients[i].wol == lpOverlapped)
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

	if (WSARecv(clients[i].socket, &clients[i].buffer, 1, &clients[i].bytes_recvd, &empty, clients[i].wol, client_read) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		perror("Error reading from client");
}

/*-------------------------------------------------------------------------
-- FUNCTION: handleRequest
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleRequest(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a request from a client and distributes it accordingly.
-------------------------------------------------------------------------*/
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
		case FILE_LIST:
			handleFileList(c);
			break;
		case FILE_REQUEST:
			handleFileRequest(c);
			break;
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: handlePlayback
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handlePlayback(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a playback command and controls the media thread.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: handleName
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleName(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a name change request from a client and notifies other clients.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: handleSelect
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleSelect(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handle a file selection request by adding the request to the media queue.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: handleUserList
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleUserList(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a request for the user list by sending it to the client.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: handleMessage
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleMessage(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a message from a client by sending it to every other client.
-------------------------------------------------------------------------*/
void handleMessage(int c)
{
	if(!clients[c].muted)
	{
		int len = *((int*)&clients[c].buffer.buf[1]) + strlen(clients[c].name) + 2;
		char *mes = (char*)malloc(len + 5);
		mes[0] = MESSAGE;
		memcpy(mes + 1, &len, sizeof(int));
		memcpy(mes + 4, clients[c].name, strlen(clients[c].name));
		mes[strlen(clients[c].name) + 5] = ':';
		mes[strlen(clients[c].name) + 6] = ' ';
		memcpy(mes + strlen(clients[c].name) + 7, clients[c].buffer.buf + 5, *((int*)&clients[c].buffer.buf[1]));
		sendMessageToAll(mes, len + 5, c);
		free(mes);
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: handleMute
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleMute(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a request for mute by checking permissions and notifying clients.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: handleFileList
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleFileList(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a request for the file list by sending it to the client.
-------------------------------------------------------------------------*/
void handleFileList(int c)
{
	printf("\nFile listing request from client[%s]\n", clients[c].name);

	for (unsigned int i = 0; i < files.size(); ++i)
	{
		int len = strlen(files[i]);
		char *mes = (char*)malloc(len + 5);
		mes[0] = FILE_LIST;
		memcpy(mes + 1, &len, sizeof(int));
		memcpy(mes + 5, files[i], len);
		sendMessage(c, mes, len + 5);
		free(mes);
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: handleFileRequest
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void handleFileRequest(int c);
--
-- PARAMETERS:
--		int c: The index of the client that initiated the request.
--
-- RETURNS: void.
--
-- NOTES:
-- Handle a file request by sending the relevant file over TCP to the client.
-------------------------------------------------------------------------*/
void handleFileRequest(int c)
{
	printf("\nFile request command from client[%s] for: %s: ", clients[c].name, clients[c].buffer.buf + 5);
	bool go = false;

	for (unsigned int i = 0; i < TYPES_LENGTH; ++i)
	{
		if (strcmp(file_types[i], strrchr(clients[c].buffer.buf + 5, '.')) == 0)
		{
			go = true;
			break;
		}
	}

	char *path = (char*)malloc(FILE_BUFF_LENGTH);
	GetCurrentDirectory(FILE_BUFF_LENGTH, path);
	std::string s2(clients[c].buffer.buf + 5);
	if (!go || s2.find(path) == std::string::npos)
	{
		printf("Denied");
		int len = strlen(FILE_DENIED_MESSAGE);
		char *mes = (char*)malloc(len + 5);
		mes[0] = MESSAGE;
		memcpy(mes + 1, &len, sizeof(len));
		sprintf(mes + 5, FILE_DENIED_MESSAGE);
		sendMessage(c, mes, len + 5);
		free(mes);
		free(path);
		return;
	}

	free(path);
	printf("Permitted");
	OFSTRUCT of;
	HFILE file = OpenFile(clients[c].buffer.buf + 5, &of, OF_READ);
	if (file == HFILE_ERROR)
	{
		printf("\nCan't open file!");
		int len = strlen(NO_OPEN_MESSAGE);
		char *mes = (char*)malloc(len + 5);
		mes[0] = MESSAGE;
		memcpy(mes + 1, &len, sizeof(len));
		sprintf(mes + 5, NO_OPEN_MESSAGE);
		sendMessage(c, mes, len + 5);
		free(mes);
	}
	else
	{
		char *mes = (char*)malloc(FILE_BUFF_LENGTH);
		DWORD bread = 0;
		int len = 0;
		mes[0] = FILE_REQUEST;

		do
		{
			ReadFile((HANDLE)file, mes + 5, FILE_BUFF_LENGTH, &bread, NULL);
			len = bread;
			memcpy(mes + 1, &len, sizeof(int));
			sendMessage(c, mes, bread + 5);
		} 
		while (bread == FILE_BUFF_LENGTH);

		len = 0;
		memcpy(mes + 1, &len, sizeof(int));
		mes[0] = FILE_EOF;
		sendMessage(c, mes, 5);
		free(mes);
	}
	CloseHandle((HANDLE)file);
}

/*-------------------------------------------------------------------------
-- FUNCTION: sendMessage
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void sendMessage(int c, char *mes, int len);
--
-- PARAMETERS:
--		int c: 		The index of the client to send the message to.
--		char *mes:	The message to send.
--		int len:	The length of the message in bytes.
--
-- RETURNS: void.
--
-- NOTES:
-- Sends a control message to a specified client.
-------------------------------------------------------------------------*/
void sendMessage(int c, char *mes, int len)
{
	send(clients[c].socket, mes, len, NULL);
}

/*-------------------------------------------------------------------------
-- FUNCTION: sendMessageToAll
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void sendMessageToAll(char *mes, int len, int except);
--
-- PARAMETERS:
--		char *mes:	The message to send.
--		int len:	The length of the message in bytes.
--		int except:	The client (if any) NOT to send the message to.
--
-- RETURNS: void.
--
-- NOTES:
-- Sends a message to every client (except an optional specified client).
-------------------------------------------------------------------------*/
void sendMessageToAll(char *mes, int len, int except)
{
	for (unsigned int i = 0; i < clients.size(); ++i)
		if (i != except)
			sendMessage(i, mes, len);
}

/*-------------------------------------------------------------------------
-- FUNCTION: checkAdmin
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: bool checkAdmin(int c);
--
-- PARAMETERS:
--		int c: The index of the client to check.
--
-- RETURNS: bool. Whether or not the user is an admin.
--
-- NOTES:
-- Checks whether or not a user is an admin.
-------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------
-- FUNCTION: findUser
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: int findUser(char *c);
--
-- PARAMETERS:
--		char *c: The name to search for.
--
-- RETURNS: int. The index of the found user, if any.
--
-- NOTES:
-- Handles a request from a client and distributes it accordingly.
-------------------------------------------------------------------------*/
int findUser(char *c)
{
	for (unsigned int i = 0; i < clients.size(); ++i)
	{
		if (strcmp(clients[i].name, c) == 0)
			return i;
	}

	return -1;
}

/*-------------------------------------------------------------------------
-- FUNCTION: mic_read
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void CALLBACK mic_read(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
--
-- PARAMETERS:
--		DWORD dwError:					The error, if any, that occurred.
--		DWORD cbTransferred:			The number of bytes that were transferred.
--		LPWSAOVERLAPPED lpOverlapped:	A pointer to the overlapped structure.
--		DWORD dwFlags:					The flags used to receive.
--
-- RETURNS: void.
--
-- NOTES:
-- Completion routine for the microphone socket.
-------------------------------------------------------------------------*/
void CALLBACK mic_read(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	for (unsigned int i = 0; i < clients.size(); ++i)
	{
		if (memcmp(&clients[i].address->sin_addr, &mic_from->sin_addr, sizeof(SOCKADDR_IN)) == 0 && !clients[i].muted)
		{
			for (unsigned int j = 0; j < cbTransferred; j += 2)
			{
				printf("Sample: %d\n", *((short*)(mic_buffer.buf + j)));
			}
		}
	}

	int mic_len = sizeof(sockaddr_in);
	if (WSARecvFrom(hMicrophone_Socket, &mic_buffer, 1, &mic_bytes_recvd, &empty, (sockaddr*)mic_from, &mic_len, &mic_wol, mic_read) == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		perror("Error reading from microphone");
}

/*-------------------------------------------------------------------------
-- FUNCTION: mixSamples
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void mixSamples(char *bus, char *channel, int num_samples);
--
-- PARAMETERS:
--		char *bus:			The first channel for samples, result will be placed here.
--		char *channel:		The second channel for samples.
--		int num_samples:	The number of samples to mix.
--
-- RETURNS: void.
--
-- NOTES:
-- Mixes two sets of PCM samples together.
-------------------------------------------------------------------------*/
void mixSamples(char *bus, char *channel, int num_samples)
{
	for (unsigned int i = 0; i < num_samples; ++i)
	{
		int additive = *((short*)(bus + (2 * i))) + *((short*)(bus + (2 * i)));
		short clipped = (additive > short_range::max()) ? short_range::max() : ((additive < short_range::min()) ? short_range::min() : additive);
		memcpy(bus + (2 * i), &clipped, sizeof(short));
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: blank_line
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void inline blank_line();
--
-- RETURNS: void.
--
-- NOTES:
-- Blanks the current line.
-------------------------------------------------------------------------*/
void inline blank_line()
{
	std::cout << '\r' << blank.c_str() << '\r';
}

/*-------------------------------------------------------------------------
-- FUNCTION: media_error
--
-- DATE: April 2nd, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void media_error(const struct libvlc_event_t* event, void *userData);
--
-- PARAMETERS:
--		const struct libvlc_event_t* event:	Pointer to the event that occurred.
--		void *userData:						Pointer to user data.
--
-- RETURNS: void.
--
-- NOTES:
-- Handles a media playing error by skipping the current media.
-------------------------------------------------------------------------*/
void media_error(const struct libvlc_event_t* event, void *userData)
{
	blank_line();
	printf("Error occurred while playing media, skipping...\n");
	skip = true;
}
