#pragma once

#include <scene.h>
#include <camera.h>

struct TestWorld: Node
{
	Model stage;
	Camera cam;
	
	Shader paintshader;
	
	void init()
	{
		setRenderTarget(main);
		
		paintshader = LoadShader("res/shader/paint.vs",
			"res/shader.paint.fs");
		
		for (int i = 0; i < stage.materialCount; i++)
		{
			stage.materials[i].shader = paintshader;
		}
	}
	
	void update()
	{
		
	}
	
	void draw()
	{
		DrawModel(stage, {0,0,0}, 1, {255,255,255,255});
	}
};