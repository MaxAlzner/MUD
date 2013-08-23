#include "../include/MUD_main.h"

#ifdef MUD_APP_H_

void OnReshape(int width, int height)
{
	if (width > height) height = int((1.0f / AspectRatio) * float(width));
	else width = int(AspectRatio * float(height));
	ScreenRect = MALib::RECT(width, height);
	glutReshapeWindow(ScreenRect.width, ScreenRect.height);
	glViewport(0, 0, ScreenRect.width, ScreenRect.height);
}
void OnFrame()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutSwapBuffers();
    glutPostRedisplay();
	UpdateFrameCount();
}
void OnUpdate()
{
}
void OnStart()
{
	glutMainLoop();
}
void OnInitialize(int argc, char **argv)
{
	MALib::LOG_Initialize(true);
	MALib::RANDOM_Initialize();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(ScreenRect.width, ScreenRect.height);
	glutInit(&argc, argv);
	glutCreateWindow("MUD");
	GLenum glError = glewInit();
	if (GLEW_OK != glError)
	{
		MALib::LOG_Message("GLEW ERROR", (const string)glewGetErrorString(glError));
	}

	glutDisplayFunc(OnFrame);
	glutReshapeFunc(OnReshape);
}
void OnUninitialize()
{
	MALib::LOG_Unitialize();
}

#endif