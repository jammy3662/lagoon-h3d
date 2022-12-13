#pragma once

#include "../engine/shader.h"
#include "player.h"

// internal output resolution
#define FWIDTH 1920
#define FHEIGHT 1080

#define ENVMAP_DIM 10

#define LOBBY_MAX 18

struct Stage
{
	Model model;
};

struct Game {

// numbers of players
int alphaCt; // team a
int betaCt; // team b
int ghostCt; // spectators

Player players[LOBBY_MAX];
Player* player;

Stage map;

Camera* activeCam;

// camera to render environment map
Camera envCam;
// camera to render shadow map
Camera sunCam;

vec3 sunPos;
float sunDist = 10;
float sunHeight = 60 * DEG2RAD; // pitch
float sunAngle = 45 * DEG2RAD; // pivot

Shader shader; // surface shader (pbr)
Shader depthShader;
Shader odepthShader;
Shader envShader; // environment map
Shader clipShader;

RenderTexture frame;
RenderTexture depth;
RenderTexture worldF;
RenderTexture playerF;
RenderTexture sunMap;

// environment map
RenderTexture envMap[6];

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
	shader = LoadShaderFromMemory(mainvsShaderCode, mainfsShaderCode);
	depthShader = LoadShaderFromMemory(mainvsShaderCode, depthfsShaderCode);
	odepthShader = LoadShaderFromMemory(mainvsShaderCode, odepthfsShaderCode);
	envShader = shader;
	
	clipShader = LoadShaderFromMemory(mainvsShaderCode, clipfsShaderCode);
	
	frame = LoadRenderTexture(FWIDTH, FHEIGHT);
	depth = LoadRenderTexture(FWIDTH, FHEIGHT);
	
	sunMap = LoadRenderTexture(FWIDTH, FHEIGHT);
	
	for (int i = 0; i < 6; i++)
	{
		envMap[i] = LoadRenderTexture(ENVMAP_DIM, ENVMAP_DIM);
	}
	
	// TODO: TESTING ONLY!
	alphaCt = 1;
	betaCt = 0;
	ghostCt = 0;
	
	for (int i = 0; i < alphaCt + betaCt + ghostCt; i++)
	{
		players[i] = Player_();
	}
	player = (players+0); // first player in array
	player->frame = LoadRenderTextureSharedDepth(frame, FWIDTH, FHEIGHT);
	
	activeCam = &player->cam.rlcam;
	
	map.model = LoadModel("res/mesh/underpass.glb");
	
	Quaternion sunRotQuat = QuaternionFromEuler(sunHeight, sunAngle, 0);
	sunPos = Vector3RotateByQuaternion (
		Vector3Scale (
			{0,0,-1}, sunDist), sunRotQuat);
	
	SetCameraMode(sunCam, CAMERA_CUSTOM);
	sunCam.up = {0,1,0};
	sunCam.projection = CAMERA_ORTHOGRAPHIC;
	sunCam.fovy = 90;
	
	SetCameraMode(envCam, CAMERA_CUSTOM);
	envCam.up = {0,1,0};
	envCam.projection = CAMERA_PERSPECTIVE;
	envCam.fovy = 90;
	
	pause(false);
}

void update()
{
	if (IsKeyPressed(kexit)) pause(!paused);
	
	player->update(!paused);
	for (int i = 1; i < alphaCt + betaCt; i++)
	{
		players[i].update(false);
	}
	
	targetRay.position = player->cam.rlcam.position;
	targetRay.direction = player->cam.lookdir;
	
	for (int i = 0; i < map.model.meshCount; i++)
	{
		Mesh m = map.model.meshes[i]; 
		targetCol = GetRayCollisionMesh(targetRay, m, MatrixIdentity());
		if (targetCol.hit) break;
	}
}

// level geometry, map elements, etc
// (any non-player geometry)
void presentWorld()
{
	DrawModel(map.model, {0,0,0}, 1, {255,255,255,255});
	
	// "reticle" //
	// TODO: move this to 2D draw section
	// (it doesnt exist yet; make it)
	if (targetCol.hit) DrawSphere(targetCol.point, 0.1, {255,0,255,255});
	else DrawSphere (
		Vector3Add (
			player->cam.rlcam.position,
			player->cam.lookdir),
		0.1, {255,0,255,255});
	
	//DrawSphere(sunPos, 1, {255,255,255,255});
}

// non-player cetas
void presentCetas()
{
	for (int i = 1; i < alphaCt + betaCt; i++)
	{
		players[i].present();
	}
}

// Depth from sun perspective //
void renderShadows()
{
	sunCam.position = Vector3Add (
		sunPos, player->position);
	sunCam.target = Vector3Add (
		player->eye(), player->cam.lookdir); 
	
	BeginTextureMode(sunMap);
	ClearBackground({255,255,255,255});
	
	BeginMode3D(sunCam);
	
	presentWorld();
	presentCetas();
// force player to draw
//	even in first person cam
	player->present(1);
	
	EndMode3D();
		
	EndTextureMode();
}

// Bake environment to reflection map //
void renderEnvironment()
{
	const vec3 directions[] =
	{
		{-1,0,0}, // west
		{ 1,0,0}, // east
		{0, 1,0}, // top
		{0,-1,0}, // bottom
		{0,0,-1}, // north
		{0,0, 1}, // south
	};
	// (this follows raylib's format)
	
	// change the up direction
	// to avoid gimbal lock
	// when facing up and down
	const vec3 updirs[] = 
	{
		{0,1,0}, // west + east
		{1,0,0}, // top + bottom
		{0,1,0}, // north + south
	};
	
	for (int i = 0; i < 6; i++)
	{
		envCam.target = Vector3Add (
			envCam.position, directions[i]);
		envCam.up = updirs[i/2];
		
		BeginTextureMode(envMap[i]);
		ClearBackground({0,0,0,255});
		
		BeginMode3D(envCam);
		
		presentWorld();
		presentCetas();
		player->present();
		
		EndMode3D();
		
		EndTextureMode();
	}
}

// Player view //
void renderPlayer()
{	
	BeginMode3D(player->cam.rlcam);
	
	player->present();
	
	EndMode3D();
}

void renderWorld()
{
	BeginMode3D(player->cam.rlcam);
	
	presentWorld();
	presentCetas();
	
	EndMode3D();
}

void renderPrep()
{
	curShader = odepthShader;
	renderShadows();
	
	// render surfaces without reflections
	curShader = envShader;
	renderEnvironment();
}

void renderMain()
{	
	// surface shader with materials
	curShader = shader;
	
	BeginTextureMode(frame);
	ClearBackground({0,0,0,255});
	renderWorld();
	EndTextureMode();
	
	BeginTextureMode(player->frame);
	rlClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	renderPlayer();
	EndTextureMode();
/*
	// render depth for clipping
	curShader = depthShader;
	
	BeginTextureMode(player->depth);
	ClearBackground({0,255,255,255});
	renderPlayer();
	EndTextureMode();
	
	BeginTextureMode(depth);
	ClearBackground({255,255,255,255});
	renderWorld();
	EndTextureMode();
	
	curShader = clipShader;
	
	shresetTextures();
	shtexture(shloc("color"), player->frame.texture, GL_TEXTURE_2D); 
	shtexture(shloc("depthSrc"), depth.texture, GL_TEXTURE_2D); 
	shtexture(shloc("depthDst"), player->depth.texture, GL_TEXTURE_2D);
*/	
	BeginTextureMode(frame);
	
	curShader = defaultShader;
	DrawTextureFlippedY(player->frame.texture, 0,0, { 255,255,255, (u8)((1 - player->opacity) * 255) });
	
	EndTextureMode();
}

void render()
{
	renderPrep();
	renderMain();
}

}; // Game
