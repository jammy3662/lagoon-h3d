#pragma once

#include "../compile/SHADER.c"
#include "../compile/TEXTURE.c"

#include "../engine/define.h"
#include "../engine/shader.h"

#include "scene.h"
#include "player.h"

// internal output resolution
#define FWIDTH 1920
#define FHEIGHT 1080

#define ENVMAP_DIM 100

#define LOBBY_MAX 18

struct Stage
{
	Model model;
};

struct Game {

// numbers of players
int alphaCt = 1; // team a
int betaCt = 0;  // team b
int ghostCt = 0; // spectators

Player players[LOBBY_MAX];
Player* player;
Player* curPlayer;

Stage map;

Camera* curCam;

// environment map camera
Camera envCam;

// shadow map camera
Camera sunCam;
Matrix sunView;
Matrix sunProj;
vec3 sunDir;
vec3 sunPos;
vec4 sunColor = {1,1,1,1};
float sunDist = 20;
float sunHeight = 45 * DEG2RAD; // pitch
float sunAngle = 225 * DEG2RAD; // pivot

Shader surfaceShader; // surface shader (pbr)
Shader envShader; // environment map
Shader fluidShader;

RenderTexture frame; // final output
RenderTexture fluidFrame;
RenderTexture sunMap;

// environment map
RenderTexture reflMap;

Ray targetRay;
RayCollision targetCol;

int paused = false;

void pause(int p)
{
	paused = p;
	
	if (p) EnableCursor();
	else DisableCursor();
}

void init()
{
	nearClip = 0.01;
	farClip = 200;
	
	surfaceShader = LoadShaderFromMemory(mainvsShaderCode, mainfsShaderCode);
	depthShader = LoadShaderFromMemory(depthvsShaderCode, depthfsShaderCode);
	envShader = LoadShaderFromMemory(mainvsShaderCode, envfsShaderCode);
	fluidShader = LoadShaderFromMemory(mainvsShaderCode, fluidfsShaderCode);
	
	frame = LoadRenderTexture(FWIDTH, FHEIGHT);
	fluidFrame = LoadRenderTextureDepth(FWIDTH, FHEIGHT);
	Player::frame = LoadRenderTextureSharedDepth(frame); // (all players will share this render target)
	
	// double frame resolution for clarity at distances
	sunMap = LoadRenderTextureDepth(FWIDTH*2, FHEIGHT*2);
	
	reflMap = LoadRenderTexture(ENVMAP_DIM, ENVMAP_DIM*6);
	
	// TODO: TESTING ONLY!
	alphaCt = 1;
	betaCt = 0;
	ghostCt = 0;
	
	// initialize players
	player = (players + 0); // first player in array is main player
	for (int i = 0; i < alphaCt + betaCt + ghostCt; i++)
	{
		players[i] = Player_();
	}
	
	curPlayer = player; // focused player is main player (could be changed for kill cam, etc)
	curCam = &player->camera();
	
	map.model = LoadModel("res/mesh/underpass.glb");
	
	Quaternion sunRotQuat = QuaternionFromEuler(sunHeight, sunAngle, 0);
	sunPos = Vector3RotateByQuaternion (
		Vector3Scale (
			{0,0,-1}, sunDist), sunRotQuat);
	
	SetCameraMode(sunCam, CAMERA_CUSTOM);
	sunCam.up = {0,1,0};
	sunCam.projection = CAMERA_ORTHOGRAPHIC;
	sunCam.fovy = 30;
	
	SetCameraMode(envCam, CAMERA_CUSTOM);
	envCam.up = {0,1,0};
	envCam.projection = CAMERA_PERSPECTIVE;
	envCam.fovy = 65;
	
	pause(true);
}

void update()
{
	if (IsKeyPressed(kexit)) pause(!paused);
	
	if (IsKeyPressed(KEY_Q)) sunAngle += 5 * DEG2RAD;
	else
	if (IsKeyPressed(KEY_E)) sunAngle -= 5 * DEG2RAD;
	
	if (IsKeyPressed(KEY_ONE)) sunHeight += 5 * DEG2RAD;
	else
	if (IsKeyPressed(KEY_THREE)) sunHeight -= 5 * DEG2RAD;
	
	player->update(!paused);
	for (int i = 1; i < alphaCt + betaCt; i++)
	{
		// block inputs to other players
		players[i].update(false);
	}
	
	targetRay.position = curCam->position;
	targetRay.direction = curPlayer->lookdir();
	
	Quaternion sunRotQuat = QuaternionFromEuler(sunHeight, sunAngle, 0);
	sunPos = Vector3RotateByQuaternion (
		Vector3Scale (
			{0,0,-1}, sunDist), sunRotQuat);
	
	for (int i = 0; i < map.model.meshCount; i++)
	{
		Mesh m = map.model.meshes[i]; 
		targetCol = GetRayCollisionMesh(targetRay, m, MatrixIdentity());
		if (targetCol.hit) break;
	}
}

// main scene
void present()
{
	DrawModel(map.model, {0,0,0}, 1, {255,255,255,255});
	
	// "reticle" //
	// TODO: move this to 2D draw section
	// (it doesnt exist yet; make it)
	if (targetCol.hit) DrawSphere(targetCol.point, 0.1, {255,0,255,255});
	else DrawSphere (
		Vector3Add (curCam->position, curPlayer->lookdir()),
		0.1, {255,0,255,255});
	
	for (int i = 1; i < alphaCt + betaCt; i++)
	{
		players[i].present();
	}
}

// Depth from sun perspective //
void genShadows()
{
	vec3 reach = Vector3Scale(
		curPlayer->lookdir(),
		(farClip - nearClip) * 0.1);
	reach = curPlayer->lookdir();
	
	vec3 focus = Vector3Add(curPlayer->eye(), reach);
	
	sunCam.position = Vector3Add(focus, sunPos);
	sunCam.target = focus;
	
	{
		RasterPass shadowPass = Raster(depthShader, sunCam, &sunMap, RasterClear::CLEAR_ALL, {1,1,1,1});
		
		//rlEnableDepthMask();
		//glDepthFunc(GL_GREATER);
		
		present();
		curPlayer->present(1);
		// force player to draw
		// even in first person cam
		
		//rlDrawRenderBatchActive();
	}
	
	//glDepthFunc(GL_LESS);
}

// Bake environment to reflection map //
void genReflections()
{
	SHADER.use(envShader);
	
	SHADER.attach("sunColor", &sunColor, SHADER_UNIFORM_VEC4);
	vec3 sunDir = Vector3Subtract(sunCam.position, sunCam.target);
	SHADER.attach("sunDir", &sunDir, SHADER_UNIFORM_VEC3);
	SHADER.attach("eye", &curCam->position, SHADER_UNIFORM_VEC3);
	
	const vec3 directions[] =
	{
		{0, 1,0}, // top
		{0,-1,0}, // bottom
		{0,0,-1}, // north
		{0,0, 1}, // south
		{ 1,0,0}, // east
		{-1,0,0}, // west
	};
	
	// change the up direction
	// to avoid gimbal lock
	// when facing up and down
	const vec3 updirs[] = 
	{
		{1,0,0}, // top + bottom
		{0,1,0}, // north + south
		{0,1,0}, // east + west
	};
	
	
	float w = reflMap.texture.width;
	float h = reflMap.texture.height;
	
	envCam.position = curPlayer->camera().position;
	
	for (int i = 0; i < 6; i++)
	{
		envCam.target = Vector3Add (
			envCam.position, directions[i]);
		envCam.up = updirs[i/2];
		
		BeginTextureMode(reflMap);
		ClearBackground({0,0,0,255});
		
		Begin3D(envCam, w, h);
		
		present();
		curPlayer->present();
		
		EndMode3D();
		
		EndTextureMode();
	}
}

// draw fluid point sprites;
// blur depth image to generate
// interpolated fluid surface
// in screen space
void genFluid()
{
	SHADER.use(fluidShader);
	
	BeginTextureMode(fluidFrame);
	ClearBackground({0,0,0,255});
	
	//
	// TODO: draw a bunch of quads corresponding
	// to the positions of each fluid particle
	//
	
	float time = GetTime();
	int t = *(float*)& time;
	srand(t);
	
	for (int i = 0; i < 20; i++)
	{
		vec3 reach = Vector3Scale(curPlayer->lookdir(), 0.1);
		vec3 target = Vector3Add(curPlayer->eye(), reach);
		
		vec3 rndOffset = {(rand()%1000)/500.0, (rand()%250)/125.0, (rand()%1000)/500.0};
		vec3 position = Vector3Add(target, rndOffset);
		
		DrawBillboardXY(position, {0.1,0.1}, *curCam);
	}
	
	EndTextureMode();
}

void render()
{
	genShadows();
	//genReflections();
	//genFluid();
	
/*
	TODO: something about this setup causes
	the shader uniforms to not bind correctly;
	shadow map produces correct output while
	main render is corrupted (missing texture) 
*/
	
	{
		RasterPass mainPass = Raster(surfaceShader, *curCam, &frame, RasterClear::CLEAR_ALL, {0,0,0,1});
		
		SHADER.attach("sunView", sunView);
		SHADER.attach("sunProj", sunProj);
		SHADER.attach("sunColor", &sunColor);
		SHADER.attach("sunDir", &sunDir);
		SHADER.attach("shadowMap", sunMap.depth, GL_TEXTURE_2D);
		SHADER.attach("reflMap", reflMap.texture, GL_TEXTURE_CUBE_MAP);
		SHADER.attach("eye", &curCam->position);
		
		present();
	}
	
	{
		// transparent background for layering;
		// leave depth buffer as-is to clip
		// against rest of scene geometry
		RasterPass mainPlayerPass = Raster(*curCam, &Player::frame, RasterClear::CLEAR_COLOR, {0,1,0,1});
		
		// dont set shader uniforms;
		// they are still there from
		// the last pass
		
		curPlayer->present();
	}
	
	BeginTextureMode(frame);
	
	SHADER.use(defaultShader);
	
	DrawTextureFlippedY(Player::frame.texture, 0,0, { 255,255,255, (u8)((1 - curPlayer->opacity) * 255) });
	//DrawTextureFlippedY(fluidFrame.depth, 0,0, {255,255,255,255});
	
	EndTextureMode();
}

}; // Game
