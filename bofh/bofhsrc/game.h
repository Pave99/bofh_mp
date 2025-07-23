#ifndef GAME_H
#define GAME_H

#include "winsock2.h"
#include "ws2tcpip.h"
#include <enet/enet.h>

#include "bofh.h"
//#include "extern.h"

struct GameState {
	ACTOR actor[MAX_ACTOR];
	int actors;
	ACTOR *localplayer;
	ENetHost* client;
	ENetPeer* peer;
	int myPlayerID;
	int hosting;
};

extern GameState gameState;


#endif

