#ifndef MUD_INPUT_H_
#define MUD_INPUT_H_

typedef struct MouseState
{
	MouseState()
	{
		memset(this, 0, sizeof(MouseState));
	}
	int x;
	int y;
	bool left;
	bool right;
	bool middle;
	bool active;
} MouseState;

#endif