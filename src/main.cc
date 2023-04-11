#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include "../include/ext.h"
#include "engine.h"
#include "player.h"

bool running = false;

GLFWwindow * window;

int resX = 1280; // window width
int resY = 720; // window height

int initRenderContext()
{
	if (!glfwInit()) {
		fprintf(stderr, "GLFW failed to initialize OpenGL\n");
		return 1;
	}
	
	// target OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(resX, resY, "lagoon-h3d test", NULL, NULL);
	
	if (!window) {
		glfwTerminate();
		fprintf(stderr, "GLFW failed to open window\n");
		return 2;
	}
	
	glfwMakeContextCurrent(window);
	
	if (!h3dInit(H3DRenderDevice::OpenGL4)) {
		fprintf(stderr, "Horde3D failed to init\n");
		return 3;
	}
	return 0;
}



int main(int argc, char* argv[])
{
	if (initRenderContext()) return 1;
	
	H3DRes pipeline = h3dAddResource(H3DResTypes::Pipeline, "h3d/gbuffer.pipeline.xml", 0);
	h3dutDumpMessages();
	
	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		//glfwSwapBuffers();
	}
	
	glfwTerminate();
	
	return 0;
}
