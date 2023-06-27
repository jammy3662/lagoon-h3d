#include <stdio.h>
#include <stdlib.h>

#include "define.h"

//#define DEBUG_INPUT

#include "backend.h"
#include "input.h"
#include "shader.h"

int main (int argc, char** argv)
{
	gpuInit ();
	
	inputInit ();
	
	Shader main = loadShaderSource ("shader/main.vs", "shader/main.fs");
	
	int paused = 0;
	int open = 1;
	
	while (gpu.open)
	{
		inputUpdate ();
		
		if (getButtonNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused)
				releaseCursor ();
			else
				captureCursor ();
		}
		
		shader (main);
		
		main["ambient"] = 0.5;
		
		gpuUpdate ();
	}
	
	gpuClose ();
	
	return 0;
}

