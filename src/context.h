#pragma once

#include "define.h"

#include "glad.h"
#include <GLFW/glfw3.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

extern GLFWwindow* window;
extern char winOpen;

extern SDL_Window* win;
extern SDL_Renderer* sdl;

extern struct res
{
	int left, top;
	union {int x,w;};
	union {int y,h;};
}
res;

void context (char* title);

// initialize global hardware context
void contextInit (const char* winTitle = "Lagoon");

// refresh gpu/hardware state
void contextRefresh ();

void refreshContext ();

// close gpu context
void contextClose ();
