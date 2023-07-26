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
	
	initGbuf ();
	
	Texture whale = loadTexture ("whale.jpg");
	Shader main = loadShader ("shader/main.vs", "shader/main.fs");
	attach (main, "ambient", 0.15);
	attach (main, "nearClip", 0.01);
	attach (main, "farClip", 1000.0);
	//Model thing = loadMeshGl ("mesh/thing.glb");
	
	int paused = 0;
	
	printf ("whale tex id %u\n", whale.id);
	
	//debugInput = 1;
	
	while (winOpen)
	{
		refreshInput ();
		
		if (buttonPressedNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused) releaseCursor ();
			else captureCursor ();
		}
		
		//useShader (main);
		
		glClearColor (0.15, 0.1, 0.2, 0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// note:	params not implemented yet
		drawTexture (whale, 0, 0, 0, 0);
		
		refresh ();
	}
	
	close ();
	
	return 0;
}

