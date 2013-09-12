#ifndef MUD_BULLET_H_
#define MUD_BULLET_H_

#ifndef MUD_PLAYER_H_
class Player;
#endif

class Bullet
{
public:
	Bullet();
	~Bullet();

	bool operator==(const Bullet& other);

	void update();
	void draw();

	void createPacket(BULLET_PACKET& packet);
	void applyPacket(BULLET_PACKET& packet);

	uint id;
	MALib::POINT forward;
	MALib::POINT start;
	MALib::RECT rect;
	MALib::COLOR color;
	BULLET_PACKET lastPacket;
};

#endif