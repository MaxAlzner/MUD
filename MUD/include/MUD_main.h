#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#include <GL\glew.h>
#include <GL\glut.h>

#include <MALib.h>
#include <MALib_sock.h>

#define MAX_CLIENTS 3u
#define MAP_WIDTH 8u
#define MAP_HEIGHT 8u
#define MAP_BUFFERSIZE 81u
#define MAP_CELLSIZE 128u
#define FRAMES_PER_SECOND 16u
#define PLAYER_TIMEOUT 30u

#include "MUD_network.h"
#include "MUD_dungeon.h"
#include "MUD_bullet.h"
#include "MUD_player.h"
#include "MUD_draw.h"
#include "MUD_def.h"
#include "MUD_callback.h"
#include "MUD_app.h"