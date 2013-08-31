#include "../include/MUD_main.h"

#ifdef MUD_DEF_H_

bool AppRunning = true;
bool Visible = false;
int WindowHandle = -1;
uint FrameCount = 0;
uint FrameRate = 0;
float DeltaTime = 0.0f;
float Theta = 0.0f;
float AspectRatio = 4.0f / 3.0f;
MALib::RECT ScreenRect(800, 600);

Dungeon* Map = NULL;
Player* Local = NULL;
MALib::ARRAY<Player*> Connected;
GAME_PACKET StatePacket;

uint Port = 20533;
string IPAddress = 0;
bool HostingGame = false;

uint ShaderProgram = 0;
uint SpriteBuffer = 0;

UniformLocations Uniforms;

float ScreenPlane[24] = 
{
	1.0f, 1.0f, 0.0f, 1.0f, 
	0.0f, 1.0f, 0.0f, 1.0f, 
	1.0f, 0.0f, 0.0f, 1.0f, 
	0.0f, 1.0f, 0.0f, 1.0f, 
	0.0f, 0.0f, 0.0f, 1.0f, 
	1.0f, 0.0f, 0.0f, 1.0f, 
};
float SpritePlane[24] = 
{
	 0.5f,  0.5f, 1.0f, 1.0f, 
	-0.5f,  0.5f, 0.0f, 1.0f, 
	 0.5f, -0.5f, 1.0f, 0.0f, 
	-0.5f,  0.5f, 0.0f, 1.0f, 
	-0.5f, -0.5f, 0.0f, 0.0f, 
	 0.5f, -0.5f, 1.0f, 0.0f, 
};

#endif