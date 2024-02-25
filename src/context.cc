#include "context.h"

#include <stdio.h>
#include <glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;
char winOpen = 1;

// viewport (window) dimensions
int viewX = 1280, viewY = 720;

struct res res = {.left = 0, .top = 0, .x = 1280, .y = 720};

void contextInit (const char* title)
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

	window = glfwCreateWindow (res.x, res.y, title, NULL, NULL);

	if (!window)
	{
		glfwTerminate ();
		fprintf (stderr, "GLFW failed to open window\n");
		return;
	}

	glfwMakeContextCurrent (window);

	gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress);
	glViewport (res.left, res.top, res.x, res.y);
}

void contextRefresh ()
{
	winOpen = !glfwWindowShouldClose (window);
	
	glfwSwapBuffers (window);
}

void contextClose ()
{
	glfwTerminate();
}