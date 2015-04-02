/*-------------------------------------------------------------------------
-- SOURCE FILE: multicast.h - Shared header for Comm Audio
--
-- PROGRAM: Comm Audio Client/Server
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

#ifndef MULTICAST_H
#define MULTICAST_H

#define MULTICAST_ADDR "234.5.6.7"
#define MULTICAST_PORT 8910
#define CONTROL_PORT 8911
#define MICROPHONE_PORT 8912

#define PACKET_SIZE 256
#define MAX_BACKLOG 5
#define MAX_ADDR_SIZE 16

typedef enum command
{
	FILE_LIST, USER_LIST, MESSAGE, FILE_SELECT, MUTE, PLAYBACK, FILE_REQUEST, SET_NAME, CURRENT, FILE_EOF
} command;

typedef enum playback
{
	PLAY, PAUSE, SKIP
} playback;

#endif
