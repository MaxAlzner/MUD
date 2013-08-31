#ifndef MUD_PLAYER_H_
#define MUD_PLAYER_H_

class Player
{
public:
	Player();
	~Player();

	void draw();

	void createPacket(PLAYER_PACKET& packet);
	void applyPacket(PLAYER_PACKET& packet);

	uint id;
	MALib::RECT rect;
	MALib::COLOR color;

	MALib::SOCKHANDLE* sock;
	PLAYER_PACKET lastPacket;
};

extern void AddPlayer(MALib::SOCKHANDLE* sock);

#endif