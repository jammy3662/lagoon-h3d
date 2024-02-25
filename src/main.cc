#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "context.h"
#include "input.h"

int main (int argc, char** argv)
{
	contextInit ("Lagoon");
	
	initInput();
	
	int paused = true;
	
	while (winOpen)
	{
		refreshInput();
		contextRefresh();
	}
	
	contextClose ();
	
	return 0;
}
