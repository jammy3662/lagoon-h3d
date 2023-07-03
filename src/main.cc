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
	gpuInit ();
	
	inputInit ();
	
	Shader main = loadShaderSource ("shader/main.vs", "shader/main.fs");
	
	Model thing = loadMesh ("mesh/thing.glb");
	
	loadMeshG ("mesh/thing.glb");
	
	int paused = 0;
	int open = 1;
	
	while (gpu.open)
	{
		inputUpdate ();
		
		if (getButtonNow (InputAction::MENU))
		{
			paused = !paused;
			
			if (paused)
				releaseCursor ();
			else
				captureCursor ();
		}
		
		shader (main);
		
		main ["ambient"] = 0.15;
		
		main ["nearClip"] = 0.01;
		main ["farClip"] = 1000.0;
		
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, thing.meshes[0].ebo);
		glDrawElements (GL_TRIANGLES, thing.meshes[0].faces.size(), GL_UNSIGNED_SHORT, thing.meshes[0].faces.data());
		
		gpuUpdate ();
	}
	
	gpuClose ();
	
	return 0;
}

