#ifndef MUD_DRAW_H_
#define MUD_DRAW_H_

struct UniformLocations
{
	int screen;
	int position;
	int dimensions;
	int rotation;
	int color_map;
	int color;
	int borderRadius;
	int shape;
};

extern string VertexShader;
extern string FragmentShader;

extern void DrawSprite(uint sprite, int x, int y, uint width, uint height, float rotation, MALib::COLOR color);
extern void DrawCircle(int x, int y, uint radius, MALib::COLOR color);
extern void DrawBox(int x, int y, uint width, uint height, float rotation, MALib::COLOR color);
extern void DrawRoundedBox(int x, int y, uint width, uint height, float borderRadius, float rotation, MALib::COLOR color);

#endif