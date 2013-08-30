#include "../include/MUD_main.h"

#ifdef MUD_PLAYER_H_

Player::Player()
{
	memset(this, 0, sizeof(Player));
	this->rect.resize(128, 128);
}
Player::~Player()
{
}

void Player::draw()
{
}

void Player::createPacket(PLAYER_PACKET& packet)
{
	packet.position.x = this->rect.cx;
	packet.position.y = this->rect.cy;
	packet.id = this->id;
	this->lastPacket = packet;
}
void Player::applyPacket(PLAYER_PACKET& packet)
{
	this->rect.cx = packet.position.x;
	this->rect.cy = packet.position.y;
}

void AddPlayer(MALib::SOCKHANDLE* sock)
{
	Player* player = new Player;
	player->sock = sock;
	Connected.add(player);
}

#endif