#include <stdio.h>
#include <stdlib.h>

#include "../include/ext.h"

#include "../compile/SHADER.c"
#include "../compile/TEXTURE.c"

#include "engine/define.h"
#include "engine/rayext.h"

#include "game/game.h"

Shader frameShader;

void drawFrame(Texture frame, Color tint)
{
	BeginShaderMode(frameShader);
	DrawTexturePro(frame, cRectangle(0,0, frame.width, -frame.height), cRectangle(0,0, wwidth, wheight), {0,0}, 0, tint);
	EndShaderMode();
}

int main(int argc, char** argv)
{
	int running = true;
	
	SetTraceLogLevel(LOG_WARNING);
	
	InitWindow(1280, 720, "sigh");
	SetTargetFPS(60);
	SetExitKey(0);
	
	frameShader = LoadShaderFromMemory(
		mainvsShaderCode, framefsShaderCode);
	
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
		
		drawFrame(game.frame.texture, {255,255,255,255});
		//drawFrame(game.lightMap.texture, {255,255,255,255});
		
		EndDrawing();
	}
	
	return 0;
}