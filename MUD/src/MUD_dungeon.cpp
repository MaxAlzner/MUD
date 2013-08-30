#include "../include/MUD_main.h"

#ifdef MUD_DUNGEON_H_

Dungeon::Dungeon()
{
	memset(this, 0, sizeof(Dungeon));
	this->rect.resize(1024, 1024);
}
Dungeon::~Dungeon()
{
}

void Dungeon::draw()
{
}

#endif