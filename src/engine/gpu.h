#pragma once

#include "define.h"

// need to include glad before GLFW
// but keep glad in the implementation
// to avoid clutter
#ifdef LGN_IMPLEMENTATION
	#include <glad.h>
#endif
#include <GLFW/glfw3.h>

extern GLFWwindow* window;
extern char winOpen;

// initialize global hardware context
void init (const char* winTitle = "Lagoon");

// refresh gpu/hardware state
void refresh ();

// close gpu context
void close ();