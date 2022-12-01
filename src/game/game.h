#pragma once

#include "player.h"

// internal output resolution
#define FWIDTH 1920
#define FHEIGHT 1080

#define LOBBY_MAX 18

struct Stage
{
	Model model;
};

struct Game
{
	Player players[LOBBY_MAX];
	
	// numbers of players
	int alphaCt; // team a
	int betaCt; // team b
	int ghostCt; // spectators
	
	Camera* cam;
	Player* player;
	
	Stage map;

	// camera to render enivronment map
	Camera3D envCam;
	// camera to render shadow map
	Camera3D lightCam;
	
	Shader shader;
	
	RenderTexture frame;
	RenderTexture lightMap;
	RenderTexture envMap; // environment map

	void init();
	void update();
	void render();
};

void Game::init()
{
	player = &players[0];
	cam = &player->cam;
	
	shader = LoadShaderFromMemory(mainvsShaderCode, mainfsShaderCode);
	
	frame = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	envMap = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	
	lightCam.projection = CAMERA_ORTHOGRAPHIC;
	lightCam.fovy = 90;
	SetCameraMode(lightCam, CAMERA_CUSTOM);
	
	envCam.projection = CAMERA_PERSPECTIVE;
	envCam.fovy = 90;
	SetCameraMode(envCam, CAMERA_CUSTOM);
}

void Game::update()
{
	if (IsKeyPressed(ktab))
	{
		cam->orbit = !cam->orbit;
	}
	
	//
	// calculate input movement vector
	//
	
	vec2 dir = {0,0};
	
	if (IsKeyDown(keast)) dir.x = -1;
	else if (IsKeyDown(kwest)) dir.x = 1;
	
	if (IsKeyDown(knorth)) dir.y = -1;
	else if (IsKeyDown(ksouth)) dir.y = 1;
	
	if (IsKeyDown(kjump))
	{
		player->move({0, player->walkSpeed, 0});
	}
	else if (IsKeyDown(kctrl))
	{
		player->move({0, -player->walkSpeed, 0});
	}
	
	dir = Vector2Rotate(dir, 2*M_PI - cam->yaw);
	dir = Vector2Normalize(dir);
	dir = Vector2Scale(dir, player->walkSpeed);
	
	player->move({dir.x, 0, dir.y});
}

void Game::render()
{
	BeginTextureMode(lightMap);
	
	BeginMode3D(lightCam);
	
	BeginShaderMode(shader);
	
	int dp = 0;
	//SetShaderValue(shader,
	//	GetShaderLocation(shader, "depth"),
	//	&dp, SHADER_UNIFORM_INT);
	
	EndShaderMode();
	
	EndMode3D();
	
	EndTextureMode();
}