#pragma once

#include "define.h"

#include "../include/glad.h"
#include <GLFW/glfw3.h>

// hardware render context
struct gpu
{
	GLFWwindow* window;

	// viewport (window) dimensions
	int viewW = 1280,
	viewH = 720;

	// frame output dimensions
	int frameW = 1920,
	frameH = 1080;

	bool open = 1;
	
	void init ()
	{
		if (!glfwInit())
		{
			fprintf (stderr, "GLFW failed to initialize OpenGL\n");
			return;
		}

		// GL 3.2 should cover old hardware but still have most modern extensions
		// (trying to be light on hardware)
		glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow (frameW, frameH, "lagoon-h3d test", NULL, NULL);

		if (!window)
		{
			glfwTerminate ();
			fprintf (stderr, "GLFW failed to open window\n");
			return;
		}

		glfwMakeContextCurrent (window);

		gladLoadGLLoader ((GLADloadproc) glfwGetProcAddress);
		glViewport (0, 0, viewW, viewH);
	}
	
	void update ()
	{
		open = !glfwWindowShouldClose (window);

		glClearColor (0.1, 0, 0.3, 1);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers (window);
	}
	
	void close ()
	{
		glfwTerminate ();
	}
	
}
gpu;

char* zFileToBuf (char* path)
{
	FILE* file = fopen (path, "r");
	if (!file) return (char*) 0;
	
	char* buffer;
	long len;
	
	fseek (file , 0, SEEK_END);
	len = ftell (file);
	rewind (file);

	// use calloc, not malloc, to ensure a 0 byte at the end
	buffer = (char*) calloc (len + 1, sizeof (char));
	fread (buffer, len, sizeof (char), file);
	
	fclose (file);
	
	return buffer;
}

// buffer must be at least 3 bytes larger than output
unsigned long b64 (unsigned char* dst, unsigned char* src, long ln)
{
	char* fmt = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned long o = 0;
	for (; ln > 0; ln -= 4)
	{
		unsigned int v = 0;
		for (int i = 0; i < 4; i++)
		{
			int c = 0;
			if (ln > 3 || i < ln)
			{
				for (;fmt [c] != src [i]; c++) if (!fmt [c]) goto r;
				o++;
			}
			v = (v << 6) | c;
		}

		v = htonl (v) >> 8;
		*((int*) dst) = v;
		src += 4;
		dst += 3;
	}
r:
	return ((o*3) >> 2);
}
