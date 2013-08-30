#ifndef MUD_DEF_H_
#define MUD_DEF_H_

#define MAX_CLIENTS 3
#define FRAMES_PER_SECOND 16

extern void UpdateFrameCount();

namespace Input
{
	extern MouseState mouse;
	extern KeyState key;
}

extern bool AppRunning;
extern bool Visible;
extern int WindowHandle;
extern uint FrameCount;
extern uint FrameRate;
extern float DeltaTime;
extern float Theta;
extern float AspectRatio;
extern MALib::RECT ScreenRect;

extern POINT Camera;

extern Player* Local;
extern MALib::ARRAY<Player*> Connected;

extern uint Port;
extern string IPAddress;
extern bool HostingGame;

extern uint ShaderProgram;
extern uint SpriteBuffer;

extern UniformLocations Uniforms;

extern float ScreenPlane[24];
extern float SpritePlane[24];

#endif