#ifndef MUD_DUNGEON_H_
#define MUD_DUNGEON_H_

class Dungeon
{
public:
	Dungeon();
	~Dungeon();

	void draw();

	MALib::RECT rect;
};

#endif