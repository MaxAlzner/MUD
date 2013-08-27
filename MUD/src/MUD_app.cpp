#include "../include/MUD_main.h"

#ifdef MUD_APP_H_

void OnStart()
{
	ChooseGameType();
	MALib::SOCK_Initialize(HostingGame);

	if (HostingGame) HostGame();
	else ConnectToGame();
	if (!AppRunning) return;

	CreateThread(NULL, 0, UpdateThread, NULL, 0, NULL);
	CreateThread(NULL, 0, NetworkThread, NULL, 0, NULL);
	glutTimerFunc(FRAMES_PER_SECOND, FrameTimer, 0);
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

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	
	int log_length = 0;
	char* buffer = new char[512];

	memset(buffer, '\0', sizeof(char) * 512);
	uint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, (const char**)&VertexShader, 0);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &log_length);
	glGetShaderInfoLog(vs, 512, &log_length, buffer);
	if (log_length > 0) printf("Vertex error log %s\n", buffer);

	memset(buffer, '\0', sizeof(char) * 512);
	uint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, (const char**)&FragmentShader, 0);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &log_length);
	glGetShaderInfoLog(fs, 512, &log_length, buffer);
	if (log_length > 0) printf("Fragment error log %s\n", buffer);

	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vs);
	glAttachShader(ShaderProgram, fs);
	
	glBindAttribLocation(ShaderProgram, 0, "vertex");
	
	int link_status = GL_FALSE;
	memset(buffer, '\0', sizeof(char) * 512);
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &link_status);
	glGetProgramiv(ShaderProgram, GL_INFO_LOG_LENGTH, &log_length);
	glGetProgramInfoLog(ShaderProgram, 512, &log_length, buffer);
	if (log_length > 0) printf("Could not link shader program %s\n", buffer);

	delete [] buffer;

	glUseProgram(ShaderProgram);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glGenBuffers(1, &SpriteBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, SpriteBuffer);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), SpritePlane, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	Uniforms.screen = glGetUniformLocation(ShaderProgram, "screen");
	Uniforms.position = glGetUniformLocation(ShaderProgram, "position");
	Uniforms.dimensions = glGetUniformLocation(ShaderProgram, "dimensions");
	Uniforms.rotation = glGetUniformLocation(ShaderProgram, "rotation");

	glutDisplayFunc(OnFrame);
	glutReshapeFunc(OnReshape);
	glutMouseFunc(OnMouseButton);
	glutPassiveMotionFunc(OnMouseMove);
	glutKeyboardFunc(OnKey);
	glutSpecialFunc(OnKeySpecial);
	glutVisibilityFunc(OnVisibility);
	glutEntryFunc(OnMouseEnter);

	WindowHandle = glutGetWindow();
}
void OnUninitialize()
{
	if (ShaderProgram != 0) glDeleteProgram(ShaderProgram);
	MALib::SOCK_Uninitialize();
	MALib::LOG_Unitialize();
}

#endif