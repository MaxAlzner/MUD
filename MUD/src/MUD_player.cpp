#include "../include/MUD_main.h"

#ifdef MUD_PLAYER_H_

Player::Player()
{
	memset(this, 0, sizeof(Player));
	this->rect.resize(80, 80);
	this->color = MALib::COLOR(1.0f, 0.25f, 0.1f);
}
Player::~Player()
{
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

void Player::createPacket(PLAYER_PACKET& packet)
{
	packet.position.x = this->rect.cx;
	packet.position.y = this->rect.cy;
	packet.id = this->id;
	packet.stillPlaying = 1;
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