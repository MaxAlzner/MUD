#ifndef MUD_NETWORK_H_
#define MUD_NETWORK_H_

extern MALib::SOCKHANDLE** Clients;

extern void ChooseGameType();

extern void HostGame();
extern void ConnectToGame();
extern void Disconnect();

extern void Communicate();

#endif