#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#define DEBUG_INPUT

#include "define.h"
#include "engine/input.h"
#include "engine/shader.h"
#include "engine/texture.h"

int main (int argc, char** argv)
{
	gpu.init ();
	
	input.init ();
	
	Texture whale = loadTexture ("whale.jpg");
	
	Shader main = loadShader ("shader/main.vs", "shader/main.fs");
	
	//Model thing = loadMeshGl ("mesh/thing.glb");
	
	int paused = 0;
	int open = 1;
	
	while (gpu.open)
	{
		input.update ();
		
		if (input.buttonNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused) input.release ();
			else input.capture ();
		}
		
		useShader (main);
		
		setUniform ("ambient", 0.15);
		
		setUniform ("nearClip", 0.01);
		setUniform ("farClip", 1000.0);
		
		gpu.update ();
	}
	
	gpu.close ();
	
	return 0;
}

