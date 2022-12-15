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
	farClip = 100;
	
	surfaceShader = LoadShaderFromMemory(mainvsShaderCode, mainfsShaderCode);
	depthShader = LoadShaderFromMemory(depthvsShaderCode, depthfsShaderCode);
	envShader = surfaceShader;
	blurShader = LoadShaderFromMemory(depthvsShaderCode, blurfsShaderCode);
	
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
	
	for (int i = 0; i < alphaCt + betaCt + ghostCt; i++)
	{
		players[i] = Player_();
	}
	player = (players + 0); // first player in array
	player->frame = LoadRenderTextureSharedDepth(frame);
	
	activeCam = &player->cam.rlcam;
	
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
	vec3 focus = Vector3Add(player->position, player->cam.lookdir);
	sunCam.position = Vector3Add(focus, sunPos);
	sunCam.target = focus; 
	
	BeginTextureMode(sunMap);
	ClearBackground({255,255,255,255});
	
	Begin3D(sunCam, FWIDTH, FHEIGHT);
	
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
		
		Begin3D(envCam, FWIDTH, FHEIGHT);
		
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
	Begin3D(player->cam.rlcam, FWIDTH, FHEIGHT);
	
	player->present();
	
	EndMode3D();
}

void renderWorld()
{
	Begin3D(player->cam.rlcam, FWIDTH, FHEIGHT);
	
	presentWorld();
	presentCetas();
	
	EndMode3D();
}

void renderPrep()
{
	shader.use(depthShader);
	renderShadows();
	
	// render surfaces without reflections
	/*
	shader.use(envShader);
	renderEnvironment();
	*/
}

void renderMain()
{
	// surface shader with materials
	shader.use(surfaceShader);
	
	Matrix sunView = MatrixLookAt(sunCam.position, sunCam.target, sunCam.up);
	Matrix sunProj = MatrixProjection(sunCam, FWIDTH, FHEIGHT);
	shader.attach("sunView", sunView);
	shader.attach("sunProj", sunProj);
	
	shader.attach("sunColor", &sunColor, SHADER_UNIFORM_VEC4);
	shader.attach("sunDir", &sunCam.position, SHADER_UNIFORM_VEC3);
	shader.attach("shadowMap", sunMap.depth, GL_TEXTURE_2D);
	
	shader.attach("eye", &player->cam.rlcam.position, SHADER_UNIFORM_VEC3);
	
	BeginTextureMode(frame);
	ClearBackground({0,0,0,255});
	renderWorld();
	EndTextureMode();
	
	BeginTextureMode(player->frame);
	// dont clear depth; sharing with main frame
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	renderPlayer();
	EndTextureMode();
	
	BeginTextureMode(frame);
	
	shader.use(defaultShader);
	DrawTextureFlippedY(player->frame.texture, 0,0, { 255,255,255, (u8)((1 - player->opacity) * 255) });
	
	EndTextureMode();
}

void render()
{
	renderPrep();
	renderMain();
}

}; // Game
