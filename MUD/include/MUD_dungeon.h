#ifndef MUD_DUNGEON_H_
#define MUD_DUNGEON_H_

typedef struct DungeonWall
{
	DungeonWall();

	uint x;
	uint y;
	MALib::RECT rect;
} DungeonWall;

class Dungeon
{
public:
	Dungeon();
	~Dungeon();

	void rebuild(uint columns, uint rows);
	void addWall(uint x, uint y);

	void draw();

	void createPacket(START_PACKET& packet);
	void applyPacket(START_PACKET& packet);
	uint fillWallBuffer(__int32* buffer, uint size);
	uint extractWallBuffer(__int32* buffer, uint count);

	uint colums;
	uint rows;
	MALib::RECT rect;
	MALib::COLOR color;
	MALib::ARRAY<DungeonWall> walls;
};

#endif