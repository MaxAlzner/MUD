#ifndef MUD_APP_H_
#define MUD_APP_H_

extern void OnReshape(int width, int height);
extern void OnFrame();
extern void OnUpdate();
extern void OnStart();
extern void OnInitialize(int argc, char **argv);
extern void OnUninitialize();

#endif