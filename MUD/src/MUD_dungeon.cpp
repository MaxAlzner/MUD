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

void Dungeon::rebuild()
{
	this->walls.zero();
	for (uint i = 0; i < MAP_WIDTH + 1; i++)
	{
		this->addWall(i, 0);
		this->addWall(i, MAP_HEIGHT);
	}
	for (uint i = 1; i < MAP_HEIGHT; i++)
	{
		this->addWall(0, i);
		this->addWall(MAP_WIDTH, i);
	}
	for (uint i = 0; i < MALib::RANDOM_Range(4u, 4 + MALib::RANDOM_D4()); i++) 
		this->addWall(MALib::RANDOM_Range(1u, MAP_WIDTH), MALib::RANDOM_Range(1u, MAP_HEIGHT));
	}
void Dungeon::addWall(uint x, uint y)
{
	if (x == MAP_WIDTH / 2 && y == MAP_HEIGHT / 2) return;
	//printf("  WALL %d, %d\n", x, y);
	DungeonWall wall;
	wall.rect += MALib::POINT(x * 128, y * 128);
	wall.id = x + (MAP_WIDTH * y);
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

#endif