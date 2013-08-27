#include "../include/MUD_main.h"

#ifdef MUD_NETWORK_H_

MALib::SOCKHANDLE** Clients = 0;

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
		printf("  error : %d\n", MALib::SOCK_CheckError());
	}
	else
	{
		printf("  Host IP : %s\n", IPAddress);
		MALib::SOCK_CreateClientBuffer(&Clients, MAX_CLIENTS);
		MALib::SOCK_AcceptConnections(Clients, MAX_CLIENTS);
	}
	printf("\n");
	_getch();
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
			printf(" Failed to connect to host\n");
			printf("  error : %d\n", MALib::SOCK_CheckError());
			continue;
		}
		
		printf(" Connected to host\n");
		printf("\n");
		break;
	}
}
void Disconnect()
{
	MALib::SOCK_Disconnect();
	if (HostingGame)
	{
		MALib::SOCK_DestroyClientBuffer(&Clients);
	}
	else
	{
		delete [] IPAddress;
	}
}

void Communicate()
{
}

#endif