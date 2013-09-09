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
		/*START_PACKET connected;
		MALib::SOCK_Receive(&connected, sizeof(START_PACKET));

		__int8* map = new __int8[connected.mapBufferSize];
		memset(map, 0, sizeof(__int8) * connected.mapBufferSize);

		MALib::SOCK_Receive(map, sizeof(__int8) * connected.mapBufferSize);

		for (int i = 0; i < connected.mapBufferSize; i++)
		{
			if (map[i] == 0) continue;
			printf("  %d %d %d\n", i, i % connected.mapWidth, i / connected.mapHeight);
			Map->addWall(i % connected.mapWidth, i / connected.mapWidth);
		}
		Local->id = connected.id;
		Local->rect.move((connected.mapWidth / 2) * connected.mapCellSize, (connected.mapHeight / 2) * connected.mapCellSize);

		delete [] map;
		
		printf("  Connected to host\n");
		printf("  I am player %d\n", Local->id);

		OnClientInitialize();*/
		break;
	}
}
void Disconnect()
{
	MALib::SOCK_Disconnect();
	/*{
		PLAYER_PACKET packet;
		Local->createPacket(packet);
		packet.stillPlaying = 0;
		MALib::SOCK_Send(&packet, sizeof(PLAYER_PACKET));
		
		MALib::SOCK_Disconnect();
		delete [] IPAddress;
	}*/
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
			player->id = AcceptedClients;
			Connected.add(player);
			printf("  Player connected\n");
			
			START_PACKET connected;
			connected.id = player->id + 1;
			connected.mapWidth = MAP_WIDTH;
			connected.mapHeight = MAP_HEIGHT;
			connected.mapCellSize = MAP_CELLSIZE;
			connected.mapBufferSize = MAP_BUFFERSIZE;
			*(__int32*)buffer = (__int32)PACKET_TYPE_ASSIGN_USER;
			buffer += sizeof(__int32);
			*((START_PACKET*)buffer) = connected;
			buffer += sizeof(START_PACKET);
			for (uint i = 0; i < Map->walls.length(); i++) buffer[Map->walls[i].id] = 1;
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

	return 100;
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

		printf("  MAP SIZE %d %d %d\n", connected.mapWidth, connected.mapHeight, connected.mapCellSize);
		for (int i = 0; i < connected.mapBufferSize; i++)
		{
			if (buffer[i] == 0) continue;
			printf("  %d %d %d\n", i, i % connected.mapWidth, i / connected.mapHeight);
			Map->addWall(i % connected.mapWidth, i / connected.mapWidth);
		}
		Local->id = connected.id;
		Local->rect.move((connected.mapWidth / 2) * connected.mapCellSize, (connected.mapHeight / 2) * connected.mapCellSize);

		printf("  Connected to host\n");
		printf("  I am player %d\n", Local->id);

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
			if (player == NULL || player->id == 0 || player->id == packet.id) continue;

			player->applyPacket(packet);
		}
		PLAYER_PACKET local;
		Local->createPacket(local);
		AddToState(state, local);
		BuildState(state);
		//printf("  CONNECTED PLAYERS %d\n", state.connected);
		if (ValidateState(state)) StatePacket = state;
	}
	else if (type == PACKET_TYPE_MASS_UPDATE && !HostingGame)
	{
		GAME_PACKET state = *((GAME_PACKET*)buffer);
		
		//printf("  CONNECTED PLAYERS %d\n", state.connected);
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

	return 100;
}

/*void PollClients()
{
	if (Clients == NULL) return;
	for (uint i = AcceptedClients; i < MAX_CLIENTS; i++)
	{
		MALib::SOCKHANDLE* sock = Clients[i];
		if (sock != NULL)
		{
			AddPlayer(sock);
			AcceptedClients++;
			printf("  Player connected\n");
			
			START_PACKET connected;
			connected.id = AcceptedClients + 1;
			connected.mapWidth = MAP_WIDTH;
			connected.mapHeight = MAP_HEIGHT;
			connected.mapCellSize = MAP_CELLSIZE;
			connected.mapBufferSize = MAP_BUFFERSIZE;

			static __int8 map[MAP_BUFFERSIZE];
			memset(map, 0, sizeof(__int8) * MAP_BUFFERSIZE);
			for (uint i = 0; i < Map->walls.length(); i++) map[Map->walls[i].id] = 1;

			MALib::SOCK_BindConnection(sock);
			MALib::SOCK_Send(&connected, sizeof(START_PACKET));
			MALib::SOCK_Send(map, sizeof(__int8) * MAP_BUFFERSIZE);
		}
	}
}
void StartCommunication()
{
}
void SendCommunicate()
{
	if (HostingGame)
	{
		for (uint i = 0; i < Connected.length(); i++)
		{
			Player* player = Connected[i];
			if (player == NULL || player->sock == NULL) continue;
			
			MALib::SOCK_BindConnection(player->sock);
			MALib::SOCK_Send(&StatePacket, sizeof(GAME_PACKET));
		}
	}
	else
	{
		PLAYER_PACKET local;
		Local->createPacket(local);

		MALib::SOCK_Send(&local, sizeof(PLAYER_PACKET));
	}
}
void ReceiveCommunicate()
{
	GAME_PACKET state;
	if (HostingGame)
	{
		//StatePacket = GAME_PACKET();
		PLAYER_PACKET local;
		Local->createPacket(local);
		for (uint i = 0; i < Connected.length(); i++)
		{
			Player* player = Connected[i];
			if (player == NULL || player->sock == NULL) continue;

			PLAYER_PACKET slot;
			MALib::SOCK_BindConnection(player->sock);
			MALib::SOCK_Receive(&slot, sizeof(PLAYER_PACKET));
			player->applyPacket(slot);
			AddToState(state, slot);
		}
		
		AddToState(state, local);
		BuildState(state);
		StatePacket = state;
	}
	else
	{
		MALib::SOCK_Receive(&state, sizeof(GAME_PACKET));

		if (!ValidateState(state)) return;
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
}*/

#endif