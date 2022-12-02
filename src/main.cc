#include <stdio.h>
#include <stdlib.h>

#include "ext.h"

#include "../compile/SHADER.c"
#include "../compile/TEXTURE.c"

#include "engine/define.h"
#include "engine/rayext.h"

#include "game/game.h"

int main(int argc, char** argv)
{
	int running = true;
	
	//SetTraceLogLevel(LOG_WARNING);
	
	InitWindow(1280, 720, "sigh");
	SetTargetFPS(60);
	SetExitKey(0);
	
	Game game;
	game.init();
	
	while (running)
	{
		if (WindowShouldClose())
		{
			printf("Exiting game...\n");
			break;
		}
		
		game.update();
		
		BeginDrawing();
		
		ClearBackground({0,0,0,255});
		
		game.render();
		
		EndDrawing();
	}
	
	return 0;
}