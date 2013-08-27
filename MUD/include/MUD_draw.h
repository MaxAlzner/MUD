#ifndef MUD_DRAW_H_
#define MUD_DRAW_H_

struct UniformLocations
{
	int screen;
	int position;
	int dimensions;
	int rotation;
};

extern string VertexShader;
extern string FragmentShader;

extern void DrawSprite(uint sprite, int x, int y, uint width, uint height, float rotation);

#endif