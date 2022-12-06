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

struct Game
{	
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
	Camera lightCam;
	
	Shader shader;
	
	RenderTexture frame;
	RenderTexture lightMap;
	
	// environment map
	RenderTexture envTop;
	RenderTexture envBottom;
	RenderTexture envNorth;
	RenderTexture envSouth;
	RenderTexture envEast;
	RenderTexture envWest;
	
	Ray targetRay;
	RayCollision targetCol;
	
	int paused = false;

	void init();
	void update();
	void render();
	
	void pause(int p);
	
	void present();
	void presentTo(RenderTexture target);
	
	void setShaderDepth(int flag, int projection);
	void setShaderReflection(int flag);
};

void Game::pause(int p)
{
	paused = p;
	
	if (p) EnableCursor();
	else DisableCursor();
}

void Game::init()
{
	shader = LoadShaderFromMemory(mainvsShaderCode, mainfsShaderCode);
	
	frame = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	
	lightMap = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	
	envTop = LoadRenderTextureWithDepthTexture(ENVMAP_DIM, ENVMAP_DIM);
	envBottom = LoadRenderTextureWithDepthTexture(ENVMAP_DIM, ENVMAP_DIM);
	envNorth = LoadRenderTextureWithDepthTexture(ENVMAP_DIM, ENVMAP_DIM);
	envSouth = LoadRenderTextureWithDepthTexture(ENVMAP_DIM, ENVMAP_DIM);
	envEast = LoadRenderTextureWithDepthTexture(ENVMAP_DIM, ENVMAP_DIM);
	envWest = LoadRenderTextureWithDepthTexture(ENVMAP_DIM, ENVMAP_DIM);
	
	// TODO: TESTING ONLY!
	alphaCt = 1;
	betaCt = 0;
	ghostCt = 0;
	
	player = &players[0];
	for (int i = 0; i < alphaCt + betaCt + ghostCt; i++)
	{
		players[i] = Player_();
	}
	
	player->focused = true;
	activeCam = &player->cam.rlcam;
	
	map.model = LoadModel("res/mesh/underpass.glb");
	shset(&map.model, shader);
	
	SetCameraMode(lightCam, CAMERA_CUSTOM);
	lightCam.up = {0,1,0};
	lightCam.projection = CAMERA_ORTHOGRAPHIC;
	lightCam.fovy = 90;
	
	SetCameraMode(envCam, CAMERA_CUSTOM);
	envCam.up = {0,1,0};
	envCam.projection = CAMERA_PERSPECTIVE;
	envCam.fovy = 90;
	
	pause(false);
}

void Game::update()
{
	if (IsKeyPressed(kexit)) pause(!paused);
	
	player->update(!paused);
	for (int i = 1; i < alphaCt + betaCt; i++)
	{
		players[i].update(false);
	}
	
	// Render "reticle" at target onscreen //
	targetRay.position = player->eye();
	targetRay.direction = player->cam.lookdir;
	if (player->cam.orbit) targetRay.direction = Vector3Add(targetRay.direction, player->cam.orbitOffset);
	targetRay.direction = Vector3Normalize(targetRay.direction);
	
	for (int i = 0; i < map.model.meshCount; i++)
	{
		Mesh m = map.model.meshes[i]; 
		targetCol = GetRayCollisionMesh(targetRay, m, MatrixIdentity());
		if (targetCol.hit) break;
	}
}

inline
void Game::setShaderDepth(int flag, int projection)
{
	int a = flag, b = projection;
	
	SetShaderValue(shader, GetShaderLocation(shader, "renderDepth"), &a, SHADER_UNIFORM_INT);
	SetShaderValue(shader, GetShaderLocation(shader, "projection"), &b, SHADER_UNIFORM_INT);
}

inline
void Game::setShaderReflection(int flag)
{
	int a = flag;
	
	SetShaderValue(shader, GetShaderLocation(shader, "renderReflection"), &a, SHADER_UNIFORM_INT);
}

// how to render a single pass of the scene
void Game::present()
{
	//BeginBlendMode(BLEND_ADD_COLORS);
	
	BeginMode3D(*activeCam);
	
	for (int i = 0; i < alphaCt + betaCt; i++)
	{
		players[i].render();
	}
	
	//DrawCube({0,1,0}, 0.1, 0.1, 0.1, {255,0,255,255});
	DrawModel(map.model, {0,0,0}, 1, {255,255,255,255});
	
	// "reticle"
	//printf("%f %f %f\n", targetCol.point.x, targetCol.point.y, targetCol.point.z);
	//DrawSphere(targetCol.point, 1, {255,0,255,255});
	DrawRay(targetRay, {255,0,0,255});
	
	rlDisableDepthTest();
	
	if (targetCol.hit) DrawSphere(targetCol.point, 0.1, {255,0,255,255});
	else DrawSphere (
		Vector3Add(player->eye(), player->cam.lookdir),
		0.1, {255,0,255,255});
	
	rlEnableDepthTest();
	
	EndMode3D();
	
	//EndBlendMode();
}

void Game::presentTo(RenderTexture target)
{
	BeginTextureMode(target);
	ClearBackground({0,0,0,255});
	present();
	EndTextureMode();
}

void Game::render()
{
	// rendering as light, so force player
	// to draw even in first person cam
	player->focused = false;
	
	// Render depth from light perspective //
	shadow_render:
	
	//setShaderDepth(true, CAMERA_ORTHOGRAPHIC);
	//setShaderDepth(true, CAMERA_PERSPECTIVE);
	
	lightCam.target = player->cam.position;
	
	vec3 lightOffset = Vector3Scale(player->lookdir, 10);
	lightCam.position =
		Vector3Subtract(player->cam.position, lightOffset);
	lightCam.projection = CAMERA_PERSPECTIVE;
	
	activeCam = &lightCam;
	
	presentTo(lightMap);
	
	// Render environment to cubemap //
	env_render:
	
	// now rendering from player perspective,
	// so only draw player in orbit cam
	player->focused = true;
	
	setShaderDepth(false, CAMERA_PERSPECTIVE);
	setShaderReflection(false);
	// dont try to render reflections
	// this cubemap has to be rendered first
	
	activeCam = &envCam;
	
	presentTo(envTop);
	presentTo(envBottom);
	presentTo(envNorth);
	presentTo(envSouth);
	presentTo(envEast);
	presentTo(envWest);
	
	main_render:
	
	setShaderReflection(true);
	
	activeCam = &player->cam.rlcam;
	presentTo(frame);
}