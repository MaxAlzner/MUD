#include "../include/MUD_main.h"

#ifdef MUD_CALLBACK_H_

void UpdateFrameCount()
{
	static time_t LastPing = ~0;
	time_t NewPing = time(0);
	FrameCount++;
	if (NewPing != LastPing)
	{
		//printf("  Frame Rate : %d\n", FrameCount);
		FrameRate = FrameCount;
		FrameCount = 0;
		LastPing = NewPing;
	}
}

void FrameTimer(int id)
{
	glutPostRedisplay();
	if (!AppRunning)
	{
		exit(0);
	}
}

ulong __stdcall UpdateThread(void* parameter)
{
	static int LastPing = 0;
	while (AppRunning)
	{
		int NewPing = glutGet(GLUT_ELAPSED_TIME);
		if (NewPing - LastPing >= FRAMES_PER_SECOND)
		{
			DeltaTime = float(NewPing - LastPing) / 1000.0f;
			OnUpdate();
			LastPing = NewPing;
		}
	}
	return 0;
}
ulong __stdcall NetworkPollThread(void* parameter)
{
	static int LastPing = 0;

	StartCommunication();
	while (AppRunning && AcceptedClients < MAX_CLIENTS)
	{
		int NewPing = glutGet(GLUT_ELAPSED_TIME);
		if (NewPing - LastPing >= 33)
		{
			PollClients();
			LastPing = NewPing;
		}
	}
	return 0;
}
ulong __stdcall NetworkSendThread(void* parameter)
{
	static int LastPing = 0;
	while (AppRunning)
	{
		int NewPing = glutGet(GLUT_ELAPSED_TIME);
		if (NewPing - LastPing >= 33)
		{
			SendCommunicate();
			LastPing = NewPing;
		}
	}
	return 0;
}
ulong __stdcall NetworkReceiveThread(void* parameter)
{
	static int LastPing = 0;
	while (AppRunning)
	{
		int NewPing = glutGet(GLUT_ELAPSED_TIME);
		if (NewPing - LastPing >= 33)
		{
			ReceiveCommunicate();
			LastPing = NewPing;
		}
	}
	return 0;
}

void OnMouseButton(int button, int state, int x, int y)
{
	bool value;
	if (state == GLUT_UP) value = false;
	else if (state == GLUT_DOWN) value = true;

	switch (button)
	{
	case GLUT_LEFT_BUTTON: MALib::INPUT::OnMouseButton(MALib::BUTTON_LEFT, value); break;
	case GLUT_RIGHT_BUTTON: MALib::INPUT::OnMouseButton(MALib::BUTTON_RIGHT, value); break;
	case GLUT_MIDDLE_BUTTON: MALib::INPUT::OnMouseButton(MALib::BUTTON_MIDDLE, value); break;
	default: break;
	}

	OnMouseMove(x, y);
}
void OnMouseMove(int x, int y)
{
	MALib::INPUT::OnMouseMove(ScreenRect.width, ScreenRect.height, x, ScreenRect.height - y);
}
void OnKey(uchar key, int x, int y)
{
	MALib::INPUT::OnKeyDown(key);

	OnMouseMove(x, y);
}
void OnKeyUp(uchar key, int x, int y)
{
	MALib::INPUT::OnKeyUp(key);

	OnMouseMove(x, y);
}
void OnKeySpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: MALib::INPUT::OnKeyDown(MALib::KEY_F1); break;
	case GLUT_KEY_F2: MALib::INPUT::OnKeyDown(MALib::KEY_F2); break;
	case GLUT_KEY_F3: MALib::INPUT::OnKeyDown(MALib::KEY_F3); break;
	case GLUT_KEY_F4: MALib::INPUT::OnKeyDown(MALib::KEY_F4); break;
	case GLUT_KEY_F5: MALib::INPUT::OnKeyDown(MALib::KEY_F5); break;
	case GLUT_KEY_F6: MALib::INPUT::OnKeyDown(MALib::KEY_F6); break;
	case GLUT_KEY_F7: MALib::INPUT::OnKeyDown(MALib::KEY_F7); break;
	case GLUT_KEY_F8: MALib::INPUT::OnKeyDown(MALib::KEY_F8); break;
	case GLUT_KEY_F9: MALib::INPUT::OnKeyDown(MALib::KEY_F9); break;
	case GLUT_KEY_F10: MALib::INPUT::OnKeyDown(MALib::KEY_F10); break;
	case GLUT_KEY_F11: MALib::INPUT::OnKeyDown(MALib::KEY_F11); break;
	case GLUT_KEY_F12: MALib::INPUT::OnKeyDown(MALib::KEY_F12); break;
	case GLUT_KEY_LEFT: MALib::INPUT::OnKeyDown(MALib::KEY_LEFT); break;
	case GLUT_KEY_UP: MALib::INPUT::OnKeyDown(MALib::KEY_UP); break;
	case GLUT_KEY_RIGHT: MALib::INPUT::OnKeyDown(MALib::KEY_RIGHT); break;
	case GLUT_KEY_DOWN: MALib::INPUT::OnKeyDown(MALib::KEY_DOWN); break;
	case GLUT_KEY_PAGE_UP: MALib::INPUT::OnKeyDown(MALib::KEY_PAGEUP); break;
	case GLUT_KEY_PAGE_DOWN: MALib::INPUT::OnKeyDown(MALib::KEY_PAGEDOWN); break;
	case GLUT_KEY_HOME: MALib::INPUT::OnKeyDown(MALib::KEY_HOME); break;
	case GLUT_KEY_INSERT: MALib::INPUT::OnKeyDown(MALib::KEY_INSERT); break;
	default: break;
	}

	OnMouseMove(x, y);
}
void OnKeySpecialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: MALib::INPUT::OnKeyUp(MALib::KEY_F1); break;
	case GLUT_KEY_F2: MALib::INPUT::OnKeyUp(MALib::KEY_F2); break;
	case GLUT_KEY_F3: MALib::INPUT::OnKeyUp(MALib::KEY_F3); break;
	case GLUT_KEY_F4: MALib::INPUT::OnKeyUp(MALib::KEY_F4); break;
	case GLUT_KEY_F5: MALib::INPUT::OnKeyUp(MALib::KEY_F5); break;
	case GLUT_KEY_F6: MALib::INPUT::OnKeyUp(MALib::KEY_F6); break;
	case GLUT_KEY_F7: MALib::INPUT::OnKeyUp(MALib::KEY_F7); break;
	case GLUT_KEY_F8: MALib::INPUT::OnKeyUp(MALib::KEY_F8); break;
	case GLUT_KEY_F9: MALib::INPUT::OnKeyUp(MALib::KEY_F9); break;
	case GLUT_KEY_F10: MALib::INPUT::OnKeyUp(MALib::KEY_F10); break;
	case GLUT_KEY_F11: MALib::INPUT::OnKeyUp(MALib::KEY_F11); break;
	case GLUT_KEY_F12: MALib::INPUT::OnKeyUp(MALib::KEY_F12); break;
	case GLUT_KEY_LEFT: MALib::INPUT::OnKeyUp(MALib::KEY_LEFT); break;
	case GLUT_KEY_UP: MALib::INPUT::OnKeyUp(MALib::KEY_UP); break;
	case GLUT_KEY_RIGHT: MALib::INPUT::OnKeyUp(MALib::KEY_RIGHT); break;
	case GLUT_KEY_DOWN: MALib::INPUT::OnKeyUp(MALib::KEY_DOWN); break;
	case GLUT_KEY_PAGE_UP: MALib::INPUT::OnKeyUp(MALib::KEY_PAGEUP); break;
	case GLUT_KEY_PAGE_DOWN: MALib::INPUT::OnKeyUp(MALib::KEY_PAGEDOWN); break;
	case GLUT_KEY_HOME: MALib::INPUT::OnKeyUp(MALib::KEY_HOME); break;
	case GLUT_KEY_INSERT: MALib::INPUT::OnKeyUp(MALib::KEY_INSERT); break;
	default: break;
	}

	OnMouseMove(x, y);
}
void OnVisibility(int state)
{
	switch (state)
	{
	case GLUT_VISIBLE: Visible = true; break;
	case GLUT_NOT_VISIBLE: Visible = false; break;
	default: break;
	}
}
void OnMouseEnter(int state)
{
	switch (state)
	{
	case GLUT_ENTERED: MALib::INPUT::OnMouseActive(true); break;
	case GLUT_LEFT: MALib::INPUT::OnMouseActive(false); break;
	default: break;
	}
}
void OnReshape(int width, int height)
{
	if (width > height) height = int((1.0f / AspectRatio) * float(width));
	else width = int(AspectRatio * float(height));

	ScreenRect.resize(width, height);

	glUniform2f(Uniforms.screen, float(ScreenRect.width), float(ScreenRect.height));
	glutReshapeWindow(ScreenRect.width, ScreenRect.height);
	glViewport(0, 0, ScreenRect.width, ScreenRect.height);
}

void OnHostInitialize()
{
	Map->rebuild();
	Local->id = 1;
	Local->rect.move((MAP_WIDTH / 2) * MAP_CELLSIZE, (MAP_HEIGHT / 2) * MAP_CELLSIZE);
}
void OnClientInitialize()
{
}
void OnFrame()
{
	static int LastPing = 0;
	int NewPing = glutGet(GLUT_ELAPSED_TIME);
	if (NewPing - LastPing >= FRAMES_PER_SECOND) LastPing = NewPing;
	else return;

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Map->draw();
	
	for (uint i = 0; i < Connected.length(); i++)
	{
		Player* player = Connected[i];
		if (player == NULL) continue;

		player->draw();
	}

	Local->draw();

    glutSwapBuffers();
    glutPostRedisplay();
	glutTimerFunc(FRAMES_PER_SECOND, FrameTimer, 0);
	UpdateFrameCount();
}
void OnUpdate()
{
	if (MALib::INPUT::GetKey(MALib::KEY_ESCAPE)) AppRunning = false;

	Theta += 1.0f;
	if (Theta >= 360.0f) Theta = 0.0f;

	int dx = 0;
	int dy = 0;
	if (MALib::INPUT::GetKey(MALib::KEY_LEFT) || MALib::INPUT::GetKey('a')) dx -= 4;
	if (MALib::INPUT::GetKey(MALib::KEY_RIGHT) || MALib::INPUT::GetKey('d')) dx += 4;
	if (MALib::INPUT::GetKey(MALib::KEY_UP) || MALib::INPUT::GetKey('w')) dy += 4;
	if (MALib::INPUT::GetKey(MALib::KEY_DOWN) || MALib::INPUT::GetKey('s')) dy -= 4;

	Local->rect.move(Local->rect.cx + dx, Local->rect.cy + dy);
	ScreenRect.move(Local->rect.cx, Local->rect.cy);

	Local->update();
}

#endif