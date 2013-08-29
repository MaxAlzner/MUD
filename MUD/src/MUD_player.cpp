#include "../include/MUD_main.h"

#ifdef MUD_PLAYER_H_

Player::Player()
{
	memset(this, 0, sizeof(Player));
}
Player::~Player()
{
}

void Player::createPacket(PLAYER_PACKET& packet)
{
	packet.position.x = this->x;
	packet.position.y = this->y;
}
void Player::applyPacket(PLAYER_PACKET& packet)
{
	this->x = packet.position.x;
	this->y = packet.position.y;
}

void AddPlayer(MALib::SOCKHANDLE* sock)
{
	Player* player = new Player;
	player->sock = sock;
	Connected.add(player);
}

#endif