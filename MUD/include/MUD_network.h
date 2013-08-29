#ifndef MUD_NETWORK_H_
#define MUD_NETWORK_H_

typedef struct PLAYER_PACKET
{
	PLAYER_PACKET()
	{
		sizeof(this, 0, sizeof(PLAYER_PACKET));
	}

	struct
	{
		__int32 x;
		__int32 y;
	} position;

	__int32 id;
	
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

extern void ChooseGameType();

extern void HostGame();
extern void ConnectToGame();
extern void Disconnect();

extern void AddToState(GAME_PACKET& state, PLAYER_PACKET& player);
extern void BuildState(GAME_PACKET& state);
extern void GetSlotFromState(GAME_PACKET& state, uint slot, PLAYER_PACKET* packet);

extern void PollClients();
extern void Communicate();

#endif