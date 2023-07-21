#define LGN_IMPLEMENTATION
#include "gpu.h"

#include <stdio.h>
#include <glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;
char winOpen = 1;

// viewport (window) dimensions
int viewX = 1280, viewY = 720;

// frame output resolution
int frameX = 1920, frameY = 1080;

void init (const char* title)
{
	if (!glfwInit())
	{
		fprintf (stderr, "GLFW failed to initialize OpenGL (hardware context)\n");
		return;
	}
	
	// GL 3.2 should cover old hardware but still have most modern extensions
	// (trying to be light on hardware)
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow (frameX, frameY, title, NULL, NULL);

	if (!window)
	{
		glfwTerminate ();
		fprintf (stderr, "GLFW failed to open window\n");
		return;
	}

	glfwMakeContextCurrent (window);

	gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress);
	glViewport (0, 0, viewX, viewY);
}

void refresh ()
{
	winOpen = !glfwWindowShouldClose (window);
	
	// TODO:	there should be a lot more here
	glClearColor (0.1, 0.1, 0.1, 1);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glfwSwapBuffers (window);
}

void close ()
{
	glfwTerminate();
}