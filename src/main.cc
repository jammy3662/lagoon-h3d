#include <stdio.h>
#include <stdlib.h>

#include "define.h"

//#define DEBUG_INPUT

#include "backend.h"
#include "input.h"
#include "shader.h"

int main (int argc, char** argv)
{
	RC render = createRenderContext ();
	
	Inputs* input = connectInput (render.window);
	
	int paused = 0;
	
	Shader main = loadShaderSource ("shader/main.vs", "shader/main.fs");
	
	while (! render.shouldClose)
	{
		updateRenderContext (&render);
		updateInputs (input);
		
		if (getButtonNow (input, InputAction::MENU))
		{
			paused = ! paused;
			
			if (paused)
				releaseCursor (input);
			else
				captureCursor (input);
		}
	}
	
	glfwTerminate ();
	
	return 0;
}

