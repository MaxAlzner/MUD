#ifndef MUD_DEF_H_
#define MUD_DEF_H_

extern void UpdateFrameCount();

extern bool AppRunning;
extern bool Visible;
extern int WindowHandle;
extern uint FrameCount;
extern uint FrameRate;
extern float DeltaTime;
extern float Theta;
extern float AspectRatio;
extern MALib::RECT ScreenRect;

extern Dungeon* Map;
extern Player* Local;
extern MALib::ARRAY<Player*> Connected;
extern GAME_PACKET StatePacket;

extern uint Port;
extern string IPAddress;
extern bool HostingGame;
extern bool Subscribed;
extern bool AssignUser;
extern bool StartedPlaying;

extern MALib::ARRAY<Bullet> LocalBullets;
extern MALib::ARRAY<Bullet> OtherBullets;

extern uint ShaderProgram;
extern uint SpriteBuffer;

extern UniformLocations Uniforms;

extern float ScreenPlane[24];
extern float SpritePlane[24];

#endif