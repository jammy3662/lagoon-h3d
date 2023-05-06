#include <stdio.h>
#include <stdlib.h>

#include "define.h"

#include "render.h"
#include "input.h"

int main (int argc, char** argv)
{
	RenderContext render =
	createRenderContext ();
	
	InputContext input
	= connectInput (render.window);
	
	int paused = 0;
	
	while (! render.shouldClose)
	{
		updateRenderContext (render);
		updateInputs (input);
		
		if (getButtonNow (input, InputAction::MENU))
		{
			paused = ! paused;
			
			if (paused)
				glfwSetInputMode (input.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode (input.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	
	glfwTerminate ();
	
	return 0;
}

