#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <engine/input.h>
#include <engine/shader.h>
#include <engine/texture.h>
#include <engine/fb.h>

int main (int argc, char** argv)
{
	init ();
	initInput ();
	initTextures ();
	
	setRes (p1080);
	
	Texture whale = loadTexture ("whale.jpg");
	
	Shader def = loadShader ("shader/main.vs", "shader/main.fs");
	/*
	attach (def, "ambient", 0.15);
	attach (def, "nearClip", 0.01);
	attach (def, "farClip", 1000.0);
	*/
	
	Frame thingB = genBuffer (640, 365);
	
	int paused = true;
	debugInput = false;
	
	releaseCursor ();
	
	while (winOpen)
	{
		refreshInput ();
		
		if (buttonPressedNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused) releaseCursor ();
			       else captureCursor ();
		}
		
		bindBuffer (thingB);
		
			glClearColor (0.15, 0.1, 0.35, 1);
			glClear (GL_COLOR_BUFFER_BIT);
			
			drawTexture (whale, {200,200}, {80,80});
		
		bindBuffer ();
		
		glClearColor (0.15, 0.1, 0.2, 1);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		drawTextureFullscreen (thingB.outputs [0]);
		//drawTextureFullscreen (whale);

		refresh ();
	}
	
	close ();
	
	return 0;
}

