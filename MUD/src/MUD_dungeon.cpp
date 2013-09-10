#include "../include/MUD_main.h"

#ifdef MUD_DUNGEON_H_

DungeonWall::DungeonWall()
{
	memset(this, 0, sizeof(DungeonWall));
	this->rect.resize(128, 128);
}

Dungeon::Dungeon()
{
	memset(this, 0, sizeof(Dungeon));
	this->color = MALib::COLOR(0.2f, 0.2f, 0.2f);
	this->walls.resize(32);
	this->rect.resize(MAP_WIDTH * 128, MAP_HEIGHT * 128);
	this->rect += MALib::POINT(MAP_WIDTH * 64, MAP_HEIGHT * 64);
}
Dungeon::~Dungeon()
{
}

void Dungeon::rebuild(uint columns, uint rows)
{
	this->colums = columns;
	this->rows = rows;
	this->walls.zero();
	for (uint i = 0; i < columns + 1; i++)
	{
		this->addWall(i, 0);
		this->addWall(i, rows);
	}
	for (uint i = 1; i < rows; i++)
	{
		this->addWall(0, i);
		this->addWall(columns, i);
	}
	for (uint i = 0; i < MALib::RANDOM_Range(4u, 4 + MALib::RANDOM_D4()); i++) 
		this->addWall(MALib::RANDOM_Range(1u, columns), MALib::RANDOM_Range(1u, rows));
}
void Dungeon::addWall(uint x, uint y)
{
	if (x == this->colums / 2 && y == this->rows / 2) return;
	//printf("  WALL %d, %d\n", x, y);
	DungeonWall wall;
	wall.rect += MALib::POINT(x * 128, y * 128);
	//wall.id = x + (MAP_WIDTH * y);
	wall.x = x;
	wall.y = y;
	this->walls.add(wall);
}

void Dungeon::draw()
{
	DrawRoundedBox(this->rect.cx, this->rect.cy, this->rect.width, this->rect.height, 0.2f, 0.0f, this->color);
	for (uint i = 0; i < this->walls.length(); i++)
	{
		DungeonWall wall = this->walls[i];

		DrawCircle(wall.rect.cx, wall.rect.cy, 64, MALib::COLOR(0.8f, 0.8f, 0.8f));
	}
}

void Dungeon::createPacket(START_PACKET& packet)
{
	packet.columns = this->colums;
	packet.rows = this->rows;
	packet.width = this->rect.width;
	packet.height = this->rect.height;
	packet.x = this->rect.cx;
	packet.y = this->rect.cy;
	packet.walls = this->walls.length();
}
void Dungeon::applyPacket(START_PACKET& packet)
{
	this->colums = packet.columns;
	this->rows = packet.rows;
	this->rect.resize(packet.width, packet.height);
	this->rect.move(packet.x, packet.y);
}
uint Dungeon::fillWallBuffer(__int32* buffer, uint size)
{
	uint count = 0;
	__int32* read = buffer;
	for (uint i = 0; i < this->walls.length(); i++)
	{
		if (count >= size) break;
		DungeonWall wall = this->walls[i];
		*read = wall.x;
		read++;
		*read = wall.y;
		read++;
		count++;
	}
	return count;
}
uint Dungeon::extractWallBuffer(__int32* buffer, uint count)
{
	uint extracted = 0;
	__int32* read = buffer;
	for (uint i = 0; i < count; i++)
	{
		__int32 x = *read;
		read++;
		__int32 y = *read;
		read++;
		this->addWall(x, y);
		extracted++;
	}
	return extracted;
}

#endif