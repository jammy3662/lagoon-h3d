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
Player* curPlayer;

Stage map;

Camera* curCam;

// environment map camera
Camera envCam;
// shadow map camera
Camera sunCam;

vec3 sunPos;
vec4 sunColor = {1,1,1,1};
float sunDist = 12;
float sunHeight = 45 * DEG2RAD; // pitch
float sunAngle = 225 * DEG2RAD; // pivot

Shader surfaceShader; // surface shader (pbr)
Shader depthShader;
Shader envShader; // environment map
Shader blurShader;

RenderTexture frame; // final output
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
	nearClip = 0.01;
	farClip = 100;
	
	surfaceShader = LoadShaderFromMemory(mainvsShaderCode, mainfsShaderCode);
	depthShader = LoadShaderFromMemory(depthvsShaderCode, depthfsShaderCode);
	envShader = surfaceShader;
	
	frame = LoadRenderTexture(FWIDTH, FHEIGHT);
	
	// double frame resolution for clarity at distances
	sunMap = LoadRenderTextureWithDepthTexture(FWIDTH*2, FHEIGHT*2);
	
	for (int i = 0; i < 6; i++)
	{
		envMap[i] = LoadRenderTexture(ENVMAP_DIM, ENVMAP_DIM);
	}
	
	// TODO: TESTING ONLY!
	alphaCt = 1;
	betaCt = 0;
	ghostCt = 0;
	
	// initialize players
	for (int i = 0; i < alphaCt + betaCt + ghostCt; i++)
	{
		players[i] = Player_();
	}
	
	player = (players + 0); // first player in array is main player
	curPlayer = player; // focused player is main player (could be changed for kill cam, etc)
	Player::frame = LoadRenderTextureSharedDepth(frame);
	// (all players will share this render target)
	
	curCam = &player->camera();
	
	map.model = LoadModel("res/mesh/underpass.glb");
	
	Quaternion sunRotQuat = QuaternionFromEuler(sunHeight, sunAngle, 0);
	sunPos = Vector3RotateByQuaternion (
		Vector3Scale (
			{0,0,-1}, sunDist), sunRotQuat);
	
	SetCameraMode(sunCam, CAMERA_CUSTOM);
	sunCam.up = {0,1,0};
	sunCam.projection = CAMERA_ORTHOGRAPHIC;
	sunCam.fovy = 20;
	
	SetCameraMode(envCam, CAMERA_CUSTOM);
	envCam.up = {0,1,0};
	envCam.projection = CAMERA_PERSPECTIVE;
	envCam.fovy = 90;
	
	pause(true);
}

void update()
{
	if (IsKeyPressed(kexit)) pause(!paused);
	
	player->update(!paused);
	for (int i = 1; i < alphaCt + betaCt; i++)
	{
		// block inputs to other players
		players[i].update(false);
	}
	
	targetRay.position = curCam->position;
	targetRay.direction = curPlayer->lookdir();
	
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
		Vector3Add (
			curCam->position,
			curPlayer->lookdir()),
		0.1, {255,0,255,255});
	
	for (int i = 1; i < alphaCt + betaCt; i++)
	{
		players[i].present();
	}
}

// Depth from sun perspective //
void genShadows()
{
	shader.use(depthShader);
	
	vec3 reach = Vector3Scale(
		curPlayer->lookdir(),
		(farClip - nearClip) * 0.5);
	
	vec3 focus = Vector3Add(curPlayer->eye(), reach);
	
	sunCam.position = Vector3Add(focus, sunPos);
	sunCam.target = focus;
	
	sunCam.position = Vector3Add(curPlayer->camera().position, sunPos);
	sunCam.target = Vector3Add(curPlayer->eye(), curPlayer->lookdir());
	
	BeginTextureMode(sunMap);
	ClearBackground({255,255,255,255});
	
	Begin3D(sunCam, FWIDTH, FHEIGHT);
	
	//rlEnableDepthMask();
	//glDepthFunc(GL_GREATER);
	
	present();
	curPlayer->present(1);
	// force player to draw
	//	even in first person cam
	
	//rlDrawRenderBatchActive();
	
	EndMode3D();
		
	EndTextureMode();
	
	//glDepthFunc(GL_LESS);
}

// Bake environment to reflection map //
void genReflections()
{
	shader.use(envShader);
	
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
	
	float w = envMap[0].texture.width;
	float h = envMap[0].texture.height;
	
	for (int i = 0; i < 6; i++)
	{
		envCam.target = Vector3Add (
			envCam.position, directions[i]);
		envCam.up = updirs[i/2];
		
		BeginTextureMode(envMap[i]);
		ClearBackground({0,0,0,255});
		
		Begin3D(envCam, w, h);
		
		present();
		player->present();
		
		EndMode3D();
		
		EndTextureMode();
	}
}

// Player view //
void renderPlayer()
{	
	
}

void renderWorld()
{
	Begin3D(curPlayer->camera(), FWIDTH, FHEIGHT);
	
	present();
	
	EndMode3D();
}

void render()
{
	genShadows();
	//genReflections();
	
	// surface shader with materials
	shader.use(surfaceShader);
	
	Matrix sunView = MatrixLookAt(sunCam.position, sunCam.target, sunCam.up);
	Matrix sunProj = MatrixProjection(sunCam, FWIDTH, FHEIGHT);
	shader.attach("sunView", sunView);
	shader.attach("sunProj", sunProj);
	
	shader.attach("sunColor", &sunColor, SHADER_UNIFORM_VEC4);
	shader.attach("sunDir", &sunCam.position, SHADER_UNIFORM_VEC3);
	shader.attach("shadowMap", sunMap.depth, GL_TEXTURE_2D);
	
	shader.attach("eye", &curCam->position, SHADER_UNIFORM_VEC3);
	
	BeginTextureMode(frame);
	ClearBackground({0,0,0,255});
	renderWorld();
	EndTextureMode();
	
	BeginTextureMode(Player::frame);
	// dont clear depth; sharing with main frame
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	Begin3D(curPlayer->camera(), FWIDTH, FHEIGHT);
	curPlayer->present();
	EndMode3D();
	
	EndTextureMode();
	
	BeginTextureMode(frame);
	
	shader.use(defaultShader);
	DrawTextureFlippedY(Player::frame.texture, 0,0, { 255,255,255, (u8)((1 - curPlayer->opacity) * 255) });
	
	EndTextureMode();
}

}; // Game
