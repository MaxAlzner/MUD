#include "../include/MUD_main.h"

#ifdef MUD_NETWORK_H_

MALib::SOCKHANDLE** Clients = 0;
uint AcceptedClients = 0;

void AddToState(GAME_PACKET& state, PLAYER_PACKET& player)
{
	switch (player.id)
	{
	case 1:
		state.players._1 = player;
		break;
	case 2:
		state.players._2 = player;
		break;
	case 3:
		state.players._3 = player; 
		break;
	case 4:
		state.players._4 = player;
		break;
	default: break;
	}
}
void BuildState(GAME_PACKET& state)
{
	state.connected = 0;
	if (state.players._1.id == 1) state.connected++;
	if (state.players._2.id == 2) state.connected++;
	if (state.players._3.id == 3) state.connected++;
	if (state.players._4.id == 4) state.connected++;
	state.bullets = LocalBullets.length();
}
bool ValidateState(GAME_PACKET& state)
{
	return state.connected <= 4 && state.connected > 0;
}
void GetPlayerFromState(GAME_PACKET& state, uint slot, PLAYER_PACKET* packet)
{
	uint index = slot - 1;
	if (packet == NULL || index > MAX_CLIENTS) return;

	PLAYER_PACKET* players = (PLAYER_PACKET*)&state.players;
	*packet = players[index];
}

void ChooseGameType()
{
	while (true)
	{
		printf("  (1). Host Game\n");
		printf("  (2). Join Game\n");
		printf("\n  >> ");
		uint input = 0;
		scanf("%d", &input);
		fflush(stdin);
		printf("\n");

		if (input == 0 || input > 2) continue;

		if (input == 1) HostingGame = true;
		else HostingGame = false;
		break;
	}
}

void Connect()
{
	while (true)
	{
		if (HostingGame)
		{
			IPAddress = MALib::SOCK_GetMyIP();
		}
		else
		{
			IPAddress = new char[64];
			printf("  IP Address : ");
			scanf("%s", IPAddress);
			fflush(stdin);
			printf("\n");
		}

		if (!MALib::SOCK_Connect(Port, IPAddress, OnNetworkCallbackSend, OnNetworkCallbackReceive))
		{
			if (HostingGame) printf("  Failed to create host\n");
			else printf("  Failed to connect to host\n");
			printf("    error : %d\n", MALib::SOCK_CheckError());
			continue;
		}
		
		if (HostingGame)
		{
			OnHostInitialize();
			printf("  IP Address : %s\n", IPAddress);
		}
		else
		{
			OnClientInitialize();
			printf("  Connected to host\n");
		}
		printf("\n");

		break;
	}
}
void Disconnect()
{
	MALib::SOCK_Disconnect();
}

void OnNetworkCullPlayers()
{
	time_t t = time(0);
	uint i = 0;
	while (i < Connected.length())
	{
		Player* player = Connected[i];
		if (player == NULL || player->lastPacket.time == 0) continue;

		uint timeout = t - player->lastPacket.time;
		if (timeout >= PLAYER_TIMEOUT) Connected.remove(player);
		i++;
	}
}
int OnNetworkCallbackSend(char* buffer, uint bytes)
{
	if (bytes < sizeof(__int32) + sizeof(GAME_PACKET)) return 0;

	if (HostingGame)
	{
		if (AssignUser && AcceptedClients < MAX_CLIENTS)
		{
			AcceptedClients++;
			AssignUser = false;
			Player* player = new Player;
			player->id = AcceptedClients + 1;
			Connected.add(player);
			printf("  Player connected : %d\n", player->id);
			
			START_PACKET connected;
			connected.id = player->id;
			Map->createPacket(connected);

			*(__int32*)buffer = (__int32)PACKET_TYPE_ASSIGN_USER;
			buffer += sizeof(__int32);
			*((START_PACKET*)buffer) = connected;
			buffer += sizeof(START_PACKET);
			Map->fillWallBuffer((__int32*)buffer, bytes);
		}
		else
		{
			*(__int32*)buffer = (__int32)PACKET_TYPE_MASS_UPDATE;
			buffer += sizeof(__int32);
			*((GAME_PACKET*)buffer) = StatePacket;
		}
	}
	else
	{
		if (!Subscribed)
		{
			Subscribed = true;
			*(__int32*)buffer = (__int32)PACKET_TYPE_SUBSCRIBE;
			buffer += sizeof(__int32);
		}
		else
		{
			*(__int32*)buffer = (__int32)PACKET_TYPE_PUSH_UPDATE;
			buffer += sizeof(__int32);
			PLAYER_PACKET local;
			Local->createPacket(local);
			*((PLAYER_PACKET*)buffer) = local;
		}
	}

	return 33;
}
int OnNetworkCallbackReceive(char* buffer, uint bytes)
{
	__int32 type = *(__int32*)buffer;
	buffer += sizeof(__int32);
	if      (type == PACKET_TYPE_SUBSCRIBE && HostingGame)
	{
		AssignUser = true;
	}
	else if (type == PACKET_TYPE_ASSIGN_USER && !HostingGame)
	{
		START_PACKET connected = *((START_PACKET*)buffer);
		buffer += sizeof(START_PACKET);

		Map->applyPacket(connected);
		Map->extractWallBuffer((__int32*)buffer, connected.walls);
		Local->id = connected.id;
		Local->rect.move(connected.x, connected.y);

		printf("  Connected to host\n");
		printf("  I am player : %d\n", Local->id);

		OnClientInitialize();
	}
	else if (type == PACKET_TYPE_PUSH_UPDATE && HostingGame)
	{
		PLAYER_PACKET packet = *((PLAYER_PACKET*)buffer);

		GAME_PACKET state = StatePacket;
		AddToState(state, packet);
		for (uint i = 0; i < Connected.length(); i++)
		{
			Player* player = Connected[i];
			if (player == NULL || player->id == 0) continue;

			if (player->id == packet.id) player->applyPacket(packet);
		}
		PLAYER_PACKET local;
		Local->createPacket(local);
		AddToState(state, local);
		BuildState(state);
		if (ValidateState(state)) StatePacket = state;
	}
	else if (type == PACKET_TYPE_MASS_UPDATE && !HostingGame)
	{
		GAME_PACKET state = *((GAME_PACKET*)buffer);
		
		if (ValidateState(state))
		{
			StatePacket = state;
		
			PLAYER_PACKET local;
			GetPlayerFromState(state, Local->id, &local);

			for (uint i = Connected.length(); i < uint(StatePacket.connected) - 1; i++) Connected.add(new Player);

			PLAYER_PACKET* players = (PLAYER_PACKET*)&StatePacket.players;
			uint k = 0;
			for (uint i = 0; i < uint(StatePacket.connected); i++)
			{
				if (k >= Connected.length()) break;
				PLAYER_PACKET packet = players[i];
				if (packet == local) continue;
				Player* player = Connected[k];
				player->applyPacket(packet);
				k++;
			}
		}
	}

	return 33;
}

#endif