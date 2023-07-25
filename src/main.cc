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
		
		useShader (main);
		
		refresh ();
	}
	
	close ();
	
	return 0;
}

