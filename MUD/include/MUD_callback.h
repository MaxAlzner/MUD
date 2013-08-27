#ifndef MUD_CALLBACK_H_
#define MUD_CALLBACK_H_

extern void UpdateFrameCount();

extern void FrameTimer(int id);

extern ulong __stdcall UpdateThread(void* parameter);
extern ulong __stdcall NetworkThread(void* parameter);

extern void OnMouseButton(int button, int state, int x, int y);
extern void OnMouseMove(int x, int y);
extern void OnKey(uchar key, int x, int y);
extern void OnKeySpecial(int key, int x, int y);
extern void OnVisibility(int state);
extern void OnMouseEnter(int state);
extern void OnReshape(int width, int height);
extern void OnFrame();
extern void OnUpdate();
extern void OnNetworkCommunication();

#endif