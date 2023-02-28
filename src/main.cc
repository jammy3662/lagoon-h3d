#include <stdio.h>
#include <stdlib.h>

#include <ext.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include "engine.h"
#include "player.h"

bool running = false;

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(1280, 720, "lagoon", 0, 0);
	glfwMakeContextCurrent(window);
	
	gladLoadGLLoader(
			(GLADloadproc)glfwGetProcAddress);
	
	h3dInit(H3DRenderDevice::OpenGL2);

	while (running)
	{
		h3dFinalizeFrame();
	}
	
	h3dRelease();

	return 0;
}
