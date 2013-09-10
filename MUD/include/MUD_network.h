#ifndef MUD_NETWORK_H_
#define MUD_NETWORK_H_

typedef enum PACKET_TYPE
{
	PACKET_TYPE_SUBSCRIBE      = 0x0FBB45CF, 
	PACKET_TYPE_ASSIGN_USER    = 0x0CAA57ED, 
	PACKET_TYPE_PUSH_UPDATE    = 0x01ABC547, 
	PACKET_TYPE_MASS_UPDATE    = 0x07FB4A8B, 
} PACKET_TYPE;

typedef struct START_PACKET
{
	START_PACKET()
	{
		memset(this, 0, sizeof(START_PACKET));
	}

	__int32 id;
	__int32 columns;
	__int32 rows;
	__int32 width;
	__int32 height;
	__int32 x;
	__int32 y;
	__int32 walls;

} CONNECTED_PACKET;
typedef struct PLAYER_PACKET
{
	PLAYER_PACKET()
	{
		sizeof(this, 0, sizeof(PLAYER_PACKET));
	}
	bool operator==(const PLAYER_PACKET& other)
	{
		return this->id == other.id && this->position.x == other.position.x && this->position.y == other.position.y;
	}

	struct
	{
		__int32 x;
		__int32 y;
	} position;

	__int32 id;
	__int32 time;
	__int8 stillPlaying;
	
} PLAYER_PACKET;
typedef struct GAME_PACKET
{
	GAME_PACKET()
	{
		memset(this, 0, sizeof(GAME_PACKET));
	}

	struct
	{
		PLAYER_PACKET _1;
		PLAYER_PACKET _2;
		PLAYER_PACKET _3;
		PLAYER_PACKET _4;
 	} players;

	__int32 connected;

} GAME_PACKET;

extern MALib::SOCKHANDLE** Clients;
extern uint AcceptedClients;

extern void AddToState(GAME_PACKET& state, PLAYER_PACKET& player);
extern void BuildState(GAME_PACKET& state);
extern bool ValidateState(GAME_PACKET& state);
extern void GetPlayerFromState(GAME_PACKET& state, uint slot, PLAYER_PACKET* packet);

extern void ChooseGameType();

extern void Connect();
extern void Disconnect();

extern void OnNetworkCullPlayers();
extern int OnNetworkCallbackSend(char* buffer, uint bytes);
extern int OnNetworkCallbackReceive(char* buffer, uint bytes);

/*extern void PollClients();
extern void StartCommunication();
extern void SendCommunicate();
extern void ReceiveCommunicate();*/

#endif