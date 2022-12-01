#include <stdio.h>
#include <stdlib.h>

#include "ext.h"

#include "engine/define.h"
#include "game/game.h"

int main(int argc, char** argv)
{
	int running = true;
	
	InitWindow(1280, 720, "sigh");
	SetTargetFPS(60);
	
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