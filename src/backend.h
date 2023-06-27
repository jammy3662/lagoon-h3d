#pragma once

#include "define.h"

#include "../include/glad.h"
#include <GLFW/glfw3.h>

// hardware render context
struct
{
	GLFWwindow* window;
	
	// viewport (window) dimensions
	int viewW = 1280,
	viewH = 720;
	
	// frame output dimensions
	int frameW = 1920,
	frameH = 1080;
	
	bool open = 1;
}
gpu;

void gpuInit ()
{
	if (! glfwInit())
	{
		fprintf (stderr, "GLFW failed to initialize OpenGL\n");
		return;
	}
	
	// GL 3.2 should cover old hardware but still have most modern extensions
	// (trying to be light on hardware)
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gpu.window = glfwCreateWindow (gpu.frameW, gpu.frameH, "lagoon-h3d test", NULL, NULL);
	
	if (!gpu.window)
	{
		glfwTerminate ();
		fprintf (stderr, "GLFW failed to open window\n");
		return;
	}
	
	glfwMakeContextCurrent (gpu.window);
	
	gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress);
	glViewport (0, 0, gpu.viewW, gpu.viewH);
}

void gpuUpdate ()
{
	gpu.open = !glfwWindowShouldClose (gpu.window);
	
	glClearColor (0.1, 0, 0.3, 1);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glfwSwapBuffers (gpu.window);
}

void gpuClose ()
{
	glfwTerminate();
}