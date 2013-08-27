#include "../include/MUD_main.h"

#ifdef MUD_CALLBACK_H_

void UpdateFrameCount()
{
	static time_t LastPing = ~0;
	time_t NewPing = time(0);
	FrameCount++;
	if (NewPing != LastPing)
	{
		printf("  Frame Rate : %d\n", FrameCount);
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
		Input::key.table[key] = false;
	}
	else if (key == 8) Input::key.value.backspace = false;
	else if (key == 27) Input::key.value.escape = false;
	else if (key == 127) Input::key.value.delet = false;

	OnMouseMove(x, y);
}
void OnKeySpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: Input::key.value.f1 = false; break;
	case GLUT_KEY_F2: Input::key.value.f2 = false; break;
	case GLUT_KEY_F3: Input::key.value.f3 = false; break;
	case GLUT_KEY_F4: Input::key.value.f4 = false; break;
	case GLUT_KEY_F5: Input::key.value.f5 = false; break;
	case GLUT_KEY_F6: Input::key.value.f6 = false; break;
	case GLUT_KEY_F7: Input::key.value.f7 = false; break;
	case GLUT_KEY_F8: Input::key.value.f8 = false; break;
	case GLUT_KEY_F9: Input::key.value.f9 = false; break;
	case GLUT_KEY_F10: Input::key.value.f10 = false; break;
	case GLUT_KEY_F11: Input::key.value.f11 = false; break;
	case GLUT_KEY_F12: Input::key.value.f12 = false; break;
	case GLUT_KEY_LEFT: Input::key.value.left = false; break;
	case GLUT_KEY_UP: Input::key.value.up = false; break;
	case GLUT_KEY_RIGHT: Input::key.value.right = false; break;
	case GLUT_KEY_DOWN: Input::key.value.down = false; break;
	case GLUT_KEY_PAGE_UP: Input::key.value.pageUp = false; break;
	case GLUT_KEY_PAGE_DOWN: Input::key.value.pageUp = false; break;
	case GLUT_KEY_HOME: Input::key.value.home = false; break;
	case GLUT_KEY_INSERT: Input::key.value.insert = false; break;
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
	
	DrawSprite(0, Input::mouse.x, Input::mouse.y, 128, 128, Theta);

    glutSwapBuffers();
    glutPostRedisplay();
	glutTimerFunc(FRAMES_PER_SECOND, FrameTimer, 0);
	UpdateFrameCount();
}
void OnUpdate()
{
	Theta += 1.0f;
	if (Theta >= 360.0f) Theta = 0.0f;
}
void OnNetworkCommunication()
{
}

#endif