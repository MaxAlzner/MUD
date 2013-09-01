#include "../include/MUD_main.h"

#ifdef MUD_DUNGEON_H_

Dungeon::Dungeon()
{
	memset(this, 0, sizeof(Dungeon));
	this->rect.resize(1024, 1024);
	this->color = MALib::COLOR(0.2f, 0.2f, 0.2f);
}
Dungeon::~Dungeon()
{
}

void Dungeon::draw()
{
	DrawRoundedBox(this->rect.cx, this->rect.cy, this->rect.width, this->rect.height, 0.2f, 0.0f, this->color);
}

#endif