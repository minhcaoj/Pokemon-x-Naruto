#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static SDL_Event gEvent;
static TTF_Font* gFont = NULL;

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

const int RENDER_DRAW_COLOR = 0xff;

#define TILE_SIZE  64
#define MAP_MAX_X  400
#define MAP_MAX_Y  10

typedef struct Map
{
	int start_x;
	int start_y;

	int max_x;
	int max_y;

	int tile[MAP_MAX_Y][MAP_MAX_X];
	char* file_name;
};



