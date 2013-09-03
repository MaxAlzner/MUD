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
}
Dungeon::~Dungeon()
{
}

void Dungeon::rebuild(uint columns, uint rows)
{
	this->rect.resize(columns * 128, rows * 128);
	this->rect += MALib::POINT(columns * 64, rows * 64);
	for (uint i = 0; i < columns + 1; i++)
	{
		this->addWall(i * 128, 0);
		this->addWall(i * 128, columns * 128);
	}
	for (uint i = 1; i < rows; i++)
	{
		this->addWall(0, i * 128);
		this->addWall(rows * 128, i * 128);
	}
	for (uint i = 0; i < MALib::RANDOM_Range(4u, 4 + MALib::RANDOM_D4()); i++) 
		this->addWall(MALib::RANDOM_Range(1u, columns) * 128, MALib::RANDOM_Range(1u, rows) * 128);
	}
void Dungeon::addWall(uint x, uint y)
{
	printf("  WALL %d, %d\n", x, y);
	DungeonWall* wall = new DungeonWall;
	wall->rect += MALib::POINT(x, y);
	this->walls.add(wall);
}

void Dungeon::draw()
{
	DrawRoundedBox(this->rect.cx, this->rect.cy, this->rect.width, this->rect.height, 0.2f, 0.0f, this->color);
	for (uint i = 0; i < this->walls.length(); i++)
	{
		DungeonWall* wall = this->walls[i];

		DrawCircle(wall->rect.cx, wall->rect.cy, 64, MALib::COLOR(0.8f, 0.8f, 0.8f));
	}
}

#endif