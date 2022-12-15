#include <stdio.h>
#include <stdlib.h>

#include "../include/ext.h"

#include "../compile/SHADER.c"
#include "../compile/TEXTURE.c"

#include "engine/define.h"
#include "engine/rayext.h"

#include "game/game.h"

/*
LOG_ALL (0)        // Display all logs
LOG_TRACE          // Trace logging, intended for internal use only
LOG_DEBUG          // Debug logging, used for internal debugging, it should be disabled on release builds
LOG_INFO (3)       // Info logging, used for program execution info
LOG_WARNING        // Warning logging, used on recoverable failures
LOG_ERROR          // Error logging, used on unrecoverable failures
LOG_FATAL (6)      // Fatal logging, used to abort program: exit(EXIT_FAILURE)
LOG_NONE            // Disable logging
*/

void drawFrame(Texture frame, Color tint)
{
	shader.use(defaultShader);
	DrawTexturePro(frame, Rectangle(0,0, frame.width, -frame.height), Rectangle(0,0, wwidth, wheight), {0,0}, 0, tint);
}

int main(int argc, char** argv)
{
	int running = true;
	
	SetTraceLogLevel(4);
	
	InitWindow(1280, 720, "sigh");
	SetTargetFPS(60);
	SetExitKey(0);
	
	defaultShader = curShader = LoadShader(0,0);
	
	Game game;
	game.init();
	
	int test = false;
	
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
		
		if (IsKeyPressed(KEY_V)) test = !test;
		
		drawFrame(game.frame.texture, {255,255,255,255});
		if (test) drawFrame(game.sunMap.depth, {255,255,255,255});
		
		EndDrawing();
	}
	
	return 0;
}