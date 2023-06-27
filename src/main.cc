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
	
	Inputs* input = connectInput (gpu.window);
	
	Shader main = loadShaderSource ("shader/main.vs", "shader/main.fs");
	
	int paused = 0;
	int open = 1;
	
	while (gpu.open)
	{
		updateInputs (input);
		
		if (getButtonNow (input, InputAction::MENU))
		{
			paused = !paused;
			
			if (paused)
				releaseCursor (input);
			else
				captureCursor (input);
		}
		
		glUseProgram (main.id);
		
		main["ambient"] = 0.5;
		
		gpuUpdate ();
	}
	
	glfwTerminate ();
	
	return 0;
}

