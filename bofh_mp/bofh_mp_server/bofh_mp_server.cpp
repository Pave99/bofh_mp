// enet_server.cpp : Defines the entry point for the console application.
//

//#include <stdio.h>

//#include <enet/enet.h>
#include <compression.h>
#include <network_protocol.h>
#include <game.h>
//#include <vector>

#undef main 

int otherPlayerID;
int nextPlayerID = 1;
int bombs;
int computers;
int terrorists;
ENetEvent event;


std::vector<uint8_t> receivedChunks;
size_t totalReceivedSize = 0;
uint32_t expectedTotalChunks=0;
uint32_t receivedChunksCount=0;
GameState ServerGameState;

int main(int argc, char* argv[])
{

	

	if(enet_initialize()!=0)
	{
		fprintf (stderr, "error occurred in initializing enet\n");
		return 1;
	}

	atexit (enet_deinitialize);

	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = 55544;

	ENetHost * server = enet_host_create (& address, 32, 2, 0, 0);

	if (server == NULL)
	{
		fprintf (stderr, "error occurred in server creating\n");
		return 1;
	}



	while (1)
	{

		while (enet_host_service (server, & event, 100) > 0) //check for events every 100ms
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
				{
					printf("SERVER: A new client connected\n");

					//Assign id

					int playerID=nextPlayerID++;
					if (!otherPlayerID)
					{
						otherPlayerID=playerID;
					}
					
					event.peer->data = (void*)playerID;

					PlayerIDMessage idMessage;
					
					idMessage.type = MSG_PLAYER_ID;
					idMessage.playerID= playerID;
					idMessage.otherPlayerID = otherPlayerID;

					printf("SERVER: sending id %d to the client\n",playerID);
					
					//printf("sizeof sent playeridmessage: %d\n",sizeof(idMessage));

					ENetPacket* packet = enet_packet_create(&idMessage, sizeof(PlayerIDMessage), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(event.peer, 1, packet);
					enet_host_flush(server);
				}
				break;
				case ENET_EVENT_TYPE_RECEIVE:
				{
					MessageType type = *(MessageType*)event.packet->data;
					printf("SERVER: received Messagetype: %d",type);
					//printf("data size: %d\n",event.packet->dataLength);
					switch(type) {
						case MSG_PLAYER_POSITION:
						{
							printf(" which is 'MSG_PLAYER_POSITION'\n");
							PlayerPosition receivedPos;

							memcpy(&receivedPos,event.packet->data,sizeof(PlayerPosition));
							//printf("sizeof received playerposition: %d\n",sizeof(receivedPos));
							//printf("type %d: player %d: Xpos: %d: Ypos: %d\n",receivedPos.type,receivedPos.playerID,receivedPos.x,receivedPos.y);
							
							PlayerPosition updatePos;

							updatePos.playerID = receivedPos.playerID;
							updatePos.x		= receivedPos.x;
							updatePos.y		= receivedPos.y;
							updatePos.angle = receivedPos.angle;
							updatePos.frame = receivedPos.frame;
							updatePos.attack= receivedPos.attack;
							updatePos.type	= MSG_PLAYER_POSITION;

							ENetPacket* packet = enet_packet_create(&updatePos, sizeof(PlayerPosition),ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
							enet_host_broadcast(server, 0, packet);
							enet_host_flush(server);
							break;
						}
						case MSG_GAME_DATA:
						{
							printf(" which is 'MSG_GAME_DATA'\n");
							GameDataMessage receivedgdm;
							memcpy(&receivedgdm,event.packet->data,sizeof(GameDataMessage));
							bombs = receivedgdm.bombs;
							computers = receivedgdm.computers;
							terrorists = receivedgdm.terrorists;
							printf("SERVER: received %d bombs, %d computers %d enemies from the host\n",bombs,computers,terrorists);
							enet_host_flush(server);
							break;
						}
						case MSG_GAME_DATA_REQUEST:
						{
							printf(" which is 'MSG_GAME_DATA_REQUEST'\n");
							GameDataRequestMessage receivedgdrm;
							memcpy(&receivedgdrm,event.packet->data,sizeof(GameDataRequestMessage));
							printf("SERVER: received game data request from client %d\n",receivedgdrm.playerID);

							GameDataMessage gdm;
							gdm.bombs = bombs;
							gdm.computers = computers;
							gdm.terrorists = terrorists;
							gdm.type = MSG_GAME_DATA;
							printf("SERVER: sending %d bombs, %d computers and %d enemies to client %d\n",gdm.bombs,gdm.computers,gdm.terrorists,receivedgdrm.playerID);
							ENetPacket* gdmpacket = enet_packet_create(&gdm, sizeof(GameDataMessage), ENET_PACKET_FLAG_RELIABLE);
							enet_peer_send(event.peer, 1,gdmpacket);
							//enet_host_flush(server);
							break;
						}
						case MSG_ACTOR_REQUEST:
						{
							printf(" which is 'MSG_ACTOR_REQUEST'\n");
							ActorRequestMessage receivedarm;
							memcpy(&receivedarm,event.packet->data,sizeof(ActorRequestMessage));
							printf("CLIENT %d: requesting actors\n",receivedarm.playerID);
							for (int i = 0; i <ServerGameState.actors; i++)
							{
								ActorData actorData;
								actorData.x = ServerGameState.actor[i].x;
								actorData.y = ServerGameState.actor[i].y;
								actorData.angle = ServerGameState.actor[i].angle;
								actorData.health = ServerGameState.actor[i].health;
								actorData.frame = ServerGameState.actor[i].frame;
								actorData.type = ServerGameState.actor[i].type;
								actorData.attack = ServerGameState.actor[i].attack;
								actorData.origin = ServerGameState.actor[i].origin;
								actorData.enemymode = ServerGameState.actor[i].enemymode;
								actorData.enemycontrol = ServerGameState.actor[i].enemycontrol;
								actorData.enemycounter = ServerGameState.actor[i].enemycounter;
								actorData.enemybored = ServerGameState.actor[i].enemybored;
								actorData.special = ServerGameState.actor[i].special;
								actorData.areanum = ServerGameState.actor[i].areanum;
								actorData.actorID = ServerGameState.actor[i].actorID;
								actorData.playerID = ServerGameState.actor[i].playerID;
								actorData.msg_type = MSG_ACTOR_DATA;
								actorData.actorcount = ServerGameState.actors;

								ENetPacket* actordatapacket = enet_packet_create(&actorData, sizeof(ActorData), ENET_PACKET_FLAG_RELIABLE);
								enet_peer_send(event.peer, 1,actordatapacket);
							}
							enet_host_flush(server);
							printf("SERVER: sent all actors to player %d\n",receivedarm.playerID);
							break;
						}
						case MSG_GAMESTATE_CHUNK:
						{
							printf(" which is 'MSG_GAMESTATE_CHUNK'\n");
							GameStateChunk receivedChunk;
							memcpy(&receivedChunk, event.packet->data, event.packet->dataLength);
							expectedTotalChunks = receivedChunk.totalChunks;
							receivedChunks.resize(expectedTotalChunks * CHUNK_SIZE);
							//printf("expected %u chunks\n",expectedTotalChunks);
							if(receivedChunk.chunkID < expectedTotalChunks)
							{
								//printf("SERVER: receiving chunk %d\n",receivedChunk.chunkID);
								memcpy(&receivedChunks[receivedChunk.chunkID * CHUNK_SIZE], receivedChunk.data, receivedChunk.dataSize);
								totalReceivedSize+=receivedChunk.dataSize;
								receivedChunksCount++;
							}

							if(receivedChunksCount == expectedTotalChunks)
							{
								printf("SERVER: all gameState chunks received\n");
								printf("SERVER: decompressing\n");
								uint8_t decompressedBuffer[GAMESTATE_SIZE];
								size_t decompressedSize = decompressGameState(&receivedChunks[0],totalReceivedSize,decompressedBuffer);
								memcpy(&ServerGameState, decompressedBuffer, decompressedSize);
								printf("SERVER: received GameState with %d actors\n",ServerGameState.actors);
							}
							break;
						}
						case MSG_ACTOR_STATE:
						{
							printf(" which is 'MSG_ACTOR_STATE'\n");
							ActorState recvdstate;
							memcpy(&recvdstate,event.packet->data,sizeof(ActorState));
														
							ActorState updateState;

							updateState.actorID		= recvdstate.actorID;
							updateState.x			= recvdstate.x;
							updateState.y			= recvdstate.y;
							updateState.angle		= recvdstate.angle;
							updateState.frame		= recvdstate.frame;
							updateState.attack		= recvdstate.attack;
							updateState.enemybored	= recvdstate.enemybored;
							updateState.enemycontrol= recvdstate.enemycontrol;
							updateState.enemycounter= recvdstate.enemycounter;
							updateState.enemymode	= recvdstate.enemymode;
							updateState.type		= MSG_ACTOR_STATE;

							ENetPacket* packet = enet_packet_create(&updateState, sizeof(ActorState),ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
							enet_host_broadcast(server, 0, packet);
							printf("SERVER: sending updated state for actor %d\n",updateState.actorID);
							enet_host_flush(server);
							break;
						}
						case MSG_ACTOR_SIGHT:
						{
							printf(" which is 'MSG_ACTOR_SIGHT'\n");
							ActorSightMessage recvdacsimsg;
							memcpy(&recvdacsimsg, event.packet->data,sizeof(ActorSightMessage));

							ENetPacket* packet = enet_packet_create(&recvdacsimsg, sizeof(ActorSightMessage),ENET_PACKET_FLAG_RELIABLE);
							enet_host_broadcast(server, 0, packet);
							printf("SERVER: broadcasting sighted actor %d\n",recvdacsimsg.actorID);
							enet_host_flush(server);
							break;
						}

					}
					break;
					//enet_packet_destroy(event.packet);
				}
				
				case ENET_EVENT_TYPE_DISCONNECT:
				{
					int disconnectedplayerid = (int)event.peer->data;
					printf("SERVER: A client with id %d disconnected\n", disconnectedplayerid);
					DisconnectMessage dm;
					dm.playerID = disconnectedplayerid;
					dm.type = MSG_DISCONNECT;
					ENetPacket* disconnectpacket = enet_packet_create(&dm, sizeof(DisconnectMessage),ENET_PACKET_FLAG_RELIABLE);
					printf("SERVER: broadcasting disconnection of client %d\n",dm.playerID);
					enet_host_broadcast(server,1,disconnectpacket);
					event.peer->data = NULL;
					
				}
				break;
				enet_packet_destroy(event.packet);
			
			}
			
		}
	}
	enet_host_destroy(server);
	return 0;
}