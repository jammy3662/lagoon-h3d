#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <engine/input.h>
#include <engine/shader.h>
#include <engine/texture.h>
#include <engine/buffer.h>

int main (int argc, char** argv)
{
	contextInit ();
	inputInit ();
	textureInit ();
	
	setRes (p1080);
	
	Texture whale = loadTexture ("whale.jpg");
	
	Shader def = loadShader ("shader/main.vs", "shader/main.fs");
	
	Frame thingB = genBuffer (128, 72);
	Frame final = genBuffer (640, 365);
	
	int paused = true;
	debugInput = false;
	
	releaseCursor ();
	
	while (winOpen)
	{
		inputRefresh ();
		
		if (buttonPressedNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused) releaseCursor ();
			       else captureCursor ();
		}
		
		bindBuffer (thingB);
		
			glClearColor (0.15, 0.1, 0.35, 1);
			glClear (GL_COLOR_BUFFER_BIT);
			
			drawTexture (whale, 0,50, 20,20);
		
		bindBuffer ();
		
		glClearColor (0.15, 0.1, 0.1, 1);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		drawFullscreen (thingB.outputs[0]);

		contextRefresh ();
	}
	
	contextClose ();
	
	return 0;
}

