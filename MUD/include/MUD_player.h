#ifndef MUD_PLAYER_H_
#define MUD_PLAYER_H_

class Player
{
public:
	Player();
	~Player();

	void update();
	void draw();

	void createPacket(PLAYER_PACKET& packet);
	void applyPacket(PLAYER_PACKET& packet);

	uint id;
	MALib::RECT rect;
	MALib::COLOR color;

	PLAYER_PACKET lastPacket;
};

#endif