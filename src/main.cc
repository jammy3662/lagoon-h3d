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
		
		if (getButton (input, InputAction::MENU))
		{
			render.shouldClose = true;
		}
	}
	
	glfwTerminate ();
	
	return 0;
}

