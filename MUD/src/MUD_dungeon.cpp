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
	if (x == this->colums && y == this->rows) return;
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