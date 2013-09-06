#ifndef MUD_DUNGEON_H_
#define MUD_DUNGEON_H_

typedef struct DungeonWall
{
	DungeonWall();

	uint id;
	MALib::RECT rect;
} DungeonWall;

class Dungeon
{
public:
	Dungeon();
	~Dungeon();

	void rebuild();
	void addWall(uint x, uint y);

	void draw();

	MALib::RECT rect;
	MALib::COLOR color;
	MALib::ARRAY<DungeonWall> walls;
};

#endif