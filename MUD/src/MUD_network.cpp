#include "../include/MUD_main.h"

#ifdef MUD_NETWORK_H_

MALib::SOCKHANDLE** Clients = 0;
uint AcceptedClients = 0;

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

void HostGame()
{
	if (!MALib::SOCK_HostConnection(Port, (string*)&IPAddress))
	{
		AppRunning = false;
		printf("  Failed to create host\n");
		printf("    error : %d\n", MALib::SOCK_CheckError());
	}
	else
	{
		printf("  Host IP : %s\n", IPAddress);
		MALib::SOCK_CreateClientBuffer(&Clients, MAX_CLIENTS);
		MALib::SOCK_AcceptConnections(Clients, MAX_CLIENTS);

		OnHostInitialize();
	}
	printf("\n");
	//_getch();
}
void ConnectToGame()
{
	IPAddress = new char[64];
	while (true)
	{
		printf("  IP Address : ");
		scanf("%s", IPAddress);
		fflush(stdin);
		//printf("  Port : ");
		//scanf("%d", Port);
		//fflush(stdin);
		printf("\n");

		if (!MALib::SOCK_ConnectTo(Port, IPAddress))
		{
			printf("  Failed to connect to host\n");
			printf("    error : %d\n", MALib::SOCK_CheckError());
			continue;
		}
		
		START_PACKET connected;
		MALib::SOCK_Receive(&connected, sizeof(START_PACKET));

		__int8* map = new __int8[connected.mapBufferSize];
		memset(map, 0, sizeof(__int8) * connected.mapBufferSize);

		MALib::SOCK_Receive(map, sizeof(__int8) * connected.mapBufferSize);

		for (int i = 0; i < connected.mapBufferSize; i++)
		{
			if (map[i] == 0) continue;
			Map->addWall(i % connected.mapWidth, i / connected.mapHeight);
		}
		Local->id = connected.id;
		Local->rect.move((connected.mapWidth / 2) * connected.mapCellSize, (connected.mapHeight / 2) * connected.mapCellSize);

		delete [] map;
		
		printf("  Connected to host\n");
		printf("  I am player %d\n", Local->id);
		printf("\n");

		OnClientInitialize();
		break;
	}
}
void Disconnect()
{
	if (HostingGame)
	{
		MALib::SOCK_Disconnect();
		MALib::SOCK_DestroyClientBuffer(&Clients);
	}
	else
	{
		PLAYER_PACKET packet;
		Local->createPacket(packet);
		packet.stillPlaying = 0;
		MALib::SOCK_Send(&packet, sizeof(PLAYER_PACKET));
		
		MALib::SOCK_Disconnect();
		delete [] IPAddress;
	}
}

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
	if (state.players._1.id == 1) state.connected++;
	if (state.players._2.id == 2) state.connected++;
	if (state.players._3.id == 3) state.connected++;
	if (state.players._4.id == 4) state.connected++;
}
bool ValidateState(GAME_PACKET& state)
{
	return state.connected <= 4;
}
void GetPlayerFromState(GAME_PACKET& state, uint slot, PLAYER_PACKET* packet)
{
	uint index = slot - 1;
	if (packet == NULL || index > MAX_CLIENTS) return;

	PLAYER_PACKET* players = (PLAYER_PACKET*)&state.players;
	*packet = players[index];
}

void PollClients()
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
			for (uint i = 0; i < Map->walls.length(); i++)
			{
				uint k = Map->walls[i]->id;
				map[k] = 1;
			}

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
}

#endif