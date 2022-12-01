#pragma once

#include <octree.h>
#include <player.h>

struct OctreeTest: Node
{
	Octree octree;
	Player player;
		
	void init()
	{
		octree.create(10, 10, 10);
		
		player.init();
	}
	
	void update()
	{
		player.update();
	}
	
	void render()
	{
		BeginMode3D(player.camera);
		
		DrawCube({0,0,3}, 0.5, 0.5, 0.5, {255,255,255,255});
		player.render();
		
		EndMode3D();
	}
};