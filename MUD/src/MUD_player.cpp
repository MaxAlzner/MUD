#include "../include/MUD_main.h"

#ifdef MUD_PLAYER_H_

Player::Player()
{
	memset(this, 0, sizeof(Player));
	this->rect.resize(128, 128);
	this->color = MALib::COLOR(1.0f, 0.25f, 0.1f);
}
Player::~Player()
{
}

void Player::draw()
{
	DrawCircle(this->rect.cx, this->rect.cy, this->rect.width, this->color);
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