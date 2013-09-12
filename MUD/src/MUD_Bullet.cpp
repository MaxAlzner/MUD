#include "../include/MUD_main.h"

#ifdef MUD_BULLET_H_

Bullet::Bullet()
{
	memset(this, 0, sizeof(Bullet));
	this->id = 0;
	this->forward = MALib::POINT(0, 1);
	this->start = MALib::POINT(0, 0);
	this->rect.resize(16, 16);
	this->color = MALib::COLOR(1.0f, 0.8f, 0.2f);
}
Bullet::~Bullet()
{
	memset(this, 0, sizeof(Bullet));
}

bool Bullet::operator==(const Bullet& other)
{
	return 
		this->id == other.id && 
		this->rect.cx == other.rect.cx && 
		this->rect.cy == other.rect.cy && 
		this->rect.width == other.rect.width && 
		this->rect.height == other.rect.height && 
		this->forward.x == other.forward.x && 
		this->forward.y == other.forward.y && 
		this->start.x == other.start.x && 
		this->start.y == other.start.y;
}

void Bullet::update()
{
	this->rect += this->forward * 8;

	float mag = MALib::Distance(this->start, MALib::POINT(this->rect.cx, this->rect.cy));
	if (mag >= 256.0f)
	{
		Destroy(this);
	}
}
void Bullet::draw()
{
	DrawCircle(this->rect.cx, this->rect.cy, this->rect.width / 2, this->color);
}

void Bullet::createPacket(BULLET_PACKET& packet)
{
	packet.position.x = this->rect.cx;
	packet.position.y = this->rect.cy;
	packet.forward.x = this->forward.x;
	packet.forward.y = this->forward.y;
	this->lastPacket = packet;
}
void Bullet::applyPacket(BULLET_PACKET& packet)
{
	this->rect.move(packet.position.x, packet.position.y);
	this->forward.x = packet.forward.x;
	this->forward.y = packet.forward.y;
	this->id = packet.id;
	this->lastPacket = packet;
}

#endif