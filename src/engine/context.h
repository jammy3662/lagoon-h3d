#pragma once

#include "define.h"

#include <glad.h>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;
extern char winOpen;

extern struct res
{
	int left, top;
	union {int x,w;};
	union {int y,h;};
}
res;

// initialize global hardware context
void contextInit (const char* winTitle = "Lagoon");

// refresh gpu/hardware state
void contextRefresh ();

// close gpu context
void contextClose ();