#pragma once

#include "define.h"

struct RenderContext
{
	GLFWwindow * window;
	
	int resX = 1280;
	int resY = 720;
	
	bool shouldClose = false;
	
	bool initialized;
};

RenderContext createRenderContext ()
{
	RenderContext render = (RenderContext) {0};
	
	if (! glfwInit())
	{
		fprintf (stderr, "GLFW failed to initialize OpenGL\n");
		return render;
	}
	
	// target OpenGL 4.0
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	render.window = glfwCreateWindow (render.resX, render.resY, "lagoon-h3d test", NULL, NULL);
	
	if (! render.window)
	{
		glfwTerminate ();
		fprintf (stderr, "GLFW failed to open window\n");
		return render;
	}
	
	glfwMakeContextCurrent (render.window);
	
	if (! h3dInit (H3DRenderDevice::OpenGL4))
	{
		fprintf (stderr, "Horde3D failed to init\n");
	}
	
	render.initialized = true;
	return render;
}

void updateRenderContext (RenderContext &	render)
{
	render.shouldClose = glfwWindowShouldClose (render.window);
	
	glfwSwapBuffers (render.window);
}