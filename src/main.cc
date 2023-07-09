#include <stdio.h>
#include <stdlib.h>

#include "define.h"

//#define DEBUG_INPUT

#include "backend.h"
#include "input.h"
#include "shader.h"
#include "asset.h"

int main (int argc, char** argv)
{
	gpu.init ();
	
	input.init ();
	
	Shader main = shader.loadFile ("shader/main.vs", "shader/main.fs");
	
	Model thing = loadMeshGl ("mesh/thing.glb");
	
	int paused = 0;
	int open = 1;
	
	while (gpu.open)
	{
		input.update ();
		
		if (input.buttonNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused) input.release ();
			else input.capture ();
		}
		
		shader.use (main);
		
		shader ["ambient"] = 0.15;
		
		shader ["nearClip"] = 0.01;
		shader ["farClip"] = 1000.0;
		
		zDrawMesh (thing.meshes [0]);
		
		gpu.update ();
	}
	
	gpu.close ();
	
	return 0;
}

