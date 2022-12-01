#pragma once

#include <camera.h>
#include <player.h>

struct LagoonFlyover: Node
{

Model island;
Shader diffuse;
Shader postproc;
Camera3D camera;
Camera cam;

uint32 time = 0;
const float scaleamount = 0.07;
const uint16 scaletime = 600; // frames

Mesh ocean;
Material oceanmat;

Player p;

void init()
{		
	postproc = LoadShader("res/shader/lighting.vs", "res/shader/lighting.fs");
	
	island = LoadModel("res/mesh/lagoon-flyover.glb");
	for (int i = 0; i < island.materialCount; i++)
	{
		island.materials[i].shader = postproc;
	}
	
	ocean = GenMeshPlane(80, 80, 100, 100);
	oceanmat = LoadMaterialDefault();
	oceanmat.shader = LoadShader("res/shader/flyover-ocean.vs", "res/shader/flyover-ocean.fs");
	
	SetCameraMode(camera, CAMERA_CUSTOM);
	camera.position = {0,8,8};
	camera.target = {0,1.8,4.5};
	camera.up = {0,1,0};
	camera.fovy = 90;
	camera.projection = CAMERA_PERSPECTIVE;
	UpdateCamera(&camera);
	
	// == camera ==
	cam.init();
	
	cam.camera.position = {0,0,0};
	
	cam.orbit = true;
	cam.orbitDist = 8;
	cam.pitch = 0.8;
	cam.orbitOffset = {0,0,-6};
	
	cam.refresh();
	
	// == player ==
	p.init();
	p.orbit = true;
	p.orbitDist = 8;
	
	DisableCursor();
}

void update()
{
	camera.position = Vector3RotateByQuaternion(camera.position, QuaternionFromEuler(0, 0.0004, 0));
	camera.target = Vector3RotateByQuaternion(camera.target, QuaternionFromEuler(0, 0.0004, 0));

	if (IsKeyPressed(KEY_D)) cam.orbitOffset.z += 1;
	if (IsKeyPressed(KEY_A)) cam.orbitOffset.z -= 1;
	
	cam.yaw += 0.0004;
	cam.refresh();
	
	p.update();
	
	time++;
	if (time < scaletime)
	{
		camera.position = Vector3Scale(camera.position,
			1 - (scaleamount / scaletime));
	}
	else if (time < scaletime * 2)
	{
		camera.position = Vector3Scale(camera.position,
			1 / (1 - (scaleamount / scaletime)));
	}
	else { time = 0; }
	
	//int loc = GetShaderLocation(oceanmat.shader, "time");
	//SetShaderValue(oceanmat.shader, loc,
	//	&time, SHADER_UNIFORM_INT);
}

void render()
{
	BeginMode3D(p.camera);
	
	DrawModel(island, {0,0,0}, 10.0, {255,255,255,255});
	DrawMesh(ocean, oceanmat, MatrixTranslate(0, 0.8, 0));
	
	EndMode3D();
	/*
	DrawRectangleRounded({200,200, 64, 28}, 0.2, 3, {0,0,0,255});
	DrawRectangleRounded({200,300, 64, 28}, 0.2, 3, {0,0,0,255});
	DrawRectangleRounded({200,400, 64, 28}, 0.2, 3, {0,0,0,255});
	DrawRectangleRounded({200,500, 64, 28}, 0.2, 3, {0,0,0,255});
	*/
}
	
};