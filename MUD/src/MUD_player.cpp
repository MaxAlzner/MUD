#include "../include/MUD_main.h"

#ifdef MUD_PLAYER_H_

Player::Player()
{
	memset(this, 0, sizeof(Player));
	this->forward = MALib::POINT(0, 1);
	this->rect.resize(80, 80);
	this->color = MALib::COLOR(1.0f, 0.25f, 0.1f);
}
Player::~Player()
{
	memset(this, 0, sizeof(Player));
}

void Player::update()
{
	for (uint i = 0; i < Map->walls.length(); i++)
	{
		DungeonWall wall = Map->walls[i];

		if (MALib::Clipping(this->rect, wall.rect))
		{
			MALib::POINT dis = MALib::Displacement(this->rect, wall.rect);
			if (dis.x == 0 && dis.y == 0) continue;

			//printf("  DISPLACEMENT %d, %d\n", dis.x, dis.y);
			//this->rect -= dis;
		}
	}
}
void Player::draw()
{
	DrawCircle(this->rect.cx, this->rect.cy, this->rect.width / 2, this->color);
}
void Player::fire()
{
	Bullet* bullet = new Bullet;
	bullet->firer = this;
	bullet->rect.move(this->rect.cx + (this->forward.x * (this->rect.width / 2)), this->rect.cy + (this->forward.y * (this->rect.height / 2)));
	bullet->start.x = bullet->rect.cx;
	bullet->start.y = bullet->rect.cy;
	bullet->forward = this->forward;
	LocalBullets.add(bullet);
}

void Player::createPacket(PLAYER_PACKET& packet)
{
	packet.position.x = this->rect.cx;
	packet.position.y = this->rect.cy;
	packet.forward.x = this->forward.x;
	packet.forward.y = this->forward.y;
	packet.id = this->id;
	packet.time = time(0);
	packet.stillPlaying = 1;
	this->lastPacket = packet;
}
void Player::applyPacket(PLAYER_PACKET& packet)
{
	this->rect.move(packet.position.x, packet.position.y);
	this->forward.x = packet.forward.x;
	this->forward.y = packet.forward.y;
	this->lastPacket = packet;
}

#endif