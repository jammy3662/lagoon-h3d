#pragma once

#include "define.h"

#include <glad.h>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;
extern char winOpen;

// initialize global hardware context
void init (const char* winTitle = "Lagoon");

// refresh gpu/hardware state
void refresh ();

// close gpu context
void close ();