#include "../include/MUD_main.h"

#ifdef MUD_DEF_H_

void UpdateFrameCount()
{
	static time_t LastPing = ~0;
	time_t NewPing = time(0);
	FrameCount++;
	if (NewPing != LastPing)
	{
		printf("  Frame Rate : %d\n", FrameCount);
		FrameCount = 0;
		LastPing = NewPing;
	}
}

uint FrameCount = 0;
float AspectRatio = 4.0f / 3.0f;
MALib::RECT ScreenRect(800, 600);

#endif