#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <engine/input.h>
#include <engine/shader.h>
#include <engine/texture.h>
#include <engine/fb.h>

int main (int argc, char** argv)
{
	init ();
	initInput ();
	initTextures ();
	
	setResolution (p1080);
	
	initGbuf ();
	
	Texture whale = loadTexture ("whale.jpg");
	
	Shader main = loadShader ("shader/main.vs", "shader/main.fs");
	attach (main, "ambient", 0.15);
	attach (main, "nearClip", 0.01);
	attach (main, "farClip", 1000.0);
	
	Frame thing = newFramebuf (1280, 720);
	Frame zip = newFramebuf (640, 365);
	
	int paused = 1;
	debugInput = 0;
	
	releaseCursor ();
	
	while (winOpen)
	{
		refreshInput ();
		
		if (buttonPressedNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused) releaseCursor ();
			else captureCursor ();
		}
		
		framebuffer (zip);
		
			glClearColor (0.15, 0.1, 0.2, 0);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			float2 sz = getFrame();
			sz.x *= 0.4; sz.y *= 0.4;
			drawTexture (whale, {0,0}, {150, 160});
			
			drawTextureFullscreen (whale);
		
		framebuffer();
		
		drawFrameFullscreen (zip, 0);
		
		refresh ();
	}
	
	close ();
	
	return 0;
}

