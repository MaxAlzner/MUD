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
	 if (!AppRunning) glutDestroyWindow(WindowHandle);
}

ulong __stdcall UpdateThread(void* parameter)
{
	static int LastPing = 0;
	while (AppRunning)
	{
		int NewPing = glutGet(GLUT_ELAPSED_TIME);
		if (NewPing - LastPing >= FRAMES_PER_SECOND)
		{
			OnUpdate();
			LastPing = NewPing;
		}
	}
	return 0;
}
ulong __stdcall NetworkThread(void* parameter)
{
	static int LastPing = 0;
	while (AppRunning)
	{
		int NewPing = glutGet(GLUT_ELAPSED_TIME);
		if (NewPing - LastPing >= 33)
		{
			OnNetworkCommunication();
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
	case GLUT_LEFT_BUTTON: Input::mouse.left = value; break;
	case GLUT_RIGHT_BUTTON: Input::mouse.right = value; break;
	case GLUT_MIDDLE_BUTTON: Input::mouse.middle = value; break;
	default: break;
	}

	OnMouseMove(x, y);
}
void OnMouseMove(int x, int y)
{
	Input::mouse.x = x;
	Input::mouse.y = ScreenRect.height - y;
}
void OnKey(uchar key, int x, int y)
{
	if (key >= ' ' && key <= '~')
	{
	}
	else if (key == 8) ;
	else if (key == 27) ;
	else if (key == 127) ;

	OnMouseMove(x, y);
}
void OnKeySpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: break;
	case GLUT_KEY_F2: break;
	case GLUT_KEY_F3: break;
	case GLUT_KEY_F4: break;
	case GLUT_KEY_F5: break;
	case GLUT_KEY_F6: break;
	case GLUT_KEY_F7: break;
	case GLUT_KEY_F8: break;
	case GLUT_KEY_F9: break;
	case GLUT_KEY_F10: break;
	case GLUT_KEY_F11: break;
	case GLUT_KEY_F12: break;
	case GLUT_KEY_LEFT: break;
	case GLUT_KEY_UP: break;
	case GLUT_KEY_RIGHT: break;
	case GLUT_KEY_DOWN: break;
	case GLUT_KEY_PAGE_UP: break;
	case GLUT_KEY_PAGE_DOWN: break;
	case GLUT_KEY_HOME: break;
	case GLUT_KEY_INSERT: break;
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
	case GLUT_ENTERED: Input::mouse.active = true;
	case GLUT_LEFT: Input::mouse.active = false;
	default: break;
	}
}
void OnReshape(int width, int height)
{
	if (width > height) height = int((1.0f / AspectRatio) * float(width));
	else width = int(AspectRatio * float(height));

	ScreenRect = MALib::RECT(width, height);

	glUniform2f(Uniforms.screen, float(width), float(height));
	glutReshapeWindow(ScreenRect.width, ScreenRect.height);
	glViewport(0, 0, ScreenRect.width, ScreenRect.height);
}
void OnFrame()
{
	static int LastPing = 0;
	int NewPing = glutGet(GLUT_ELAPSED_TIME);
	if (NewPing - LastPing >= FRAMES_PER_SECOND) LastPing = NewPing;
	else return;

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (uint i = 0; i < Connected.length(); i++)
	{
		Player* player = Connected[i];
		if (player == NULL) continue;

		DrawSprite(0, player->x, player->y, 128, 128, 0.0f);
	}

	DrawSprite(0, Local->x, Local->y, 128, 128, 0.0f);

    glutSwapBuffers();
    glutPostRedisplay();
	glutTimerFunc(FRAMES_PER_SECOND, FrameTimer, 0);
	UpdateFrameCount();
}
void OnUpdate()
{
	Theta += 1.0f;
	if (Theta >= 360.0f) Theta = 0.0f;

	Local->x = Input::mouse.x;
	Local->y = Input::mouse.y;
}
void OnNetworkCommunication()
{
	if (HostingGame)
	{
		PollClients();
	}

	Communicate();
}

#endif