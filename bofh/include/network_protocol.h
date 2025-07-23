//  networking

#include <vector>
#include "game.h"
#include <inttypes.h>

#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

typedef uint8_t MessageType;
enum {
	MSG_UNUSED,
	MSG_PLAYER_POSITION,
	MSG_PLAYER_ID,
	MSG_ACTOR_REQUEST,
	MSG_ACTOR_DATA,
	MSG_GAME_DATA_REQUEST,
	MSG_GAME_DATA,
	MSG_GAMESTATE,
	MSG_DISCONNECT,
	MSG_GAMESTATE_CHUNK,
	MSG_ACTOR_STATE,
	MSG_ACTOR_SIGHT
};

struct PlayerPosition {
  MessageType type;
  int playerID;
  int x;
  int y;
  int angle;
  int frame;
  int attack;
};

struct ActorState {
  MessageType type;
  int actorID;
  int x;
  int y;
  int angle;
  int frame;
  int attack;
  int enemymode;
  int enemycontrol;
  int enemycounter;
  int enemybored;
  int attackdelay;
};

struct ActorRequestMessage {
	MessageType type;
	int playerID;
};

struct ActorData {
	MessageType msg_type;
	int x;
	int y;
	int angle;
	int angularspeed;
	int speedx;
	int speedy;
	int health;
	int frame;
	int type;
	int attack;
	int attackdelay;
	void *origin;
	int enemymode;
	int enemycontrol;
	int enemycounter;
    int enemybored;
	int special; /* Number of grenades for enemies */
    int areanum; /* For collision detection */
	int playerID;
	int actorID;
	int actorcount;
};

struct GameDataRequestMessage {
	MessageType type;
	int playerID;
};

struct GameDataMessage {
	MessageType type;
	int terrorists;
	int bombs;
	int computers;
};

struct GameStateMessage {
	MessageType type;
	GameState gameState;
};

struct GameStateChunk {
	MessageType type;
	uint32_t chunkID;
	uint32_t totalChunks;
	uint32_t dataSize;
	uint8_t data[1000];

};

/*struct GameStateChunkRequest {
	MessageType type;
	uint32_t totalChunks;
};*/

struct PlayerIDMessage{
	MessageType type;
	int playerID;
	int otherPlayerID;
};

struct DisconnectMessage{
	MessageType type;
	int playerID;
};

struct ActorSightMessage{
	MessageType type;
	int playerID;
	int actorID;
};


#endif


