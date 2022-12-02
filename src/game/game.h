#pragma once

#include "../engine/shader.h"
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

	void init();
	void update();
	void render();
	
	void present(Camera camera);
	
	void setShaderDepth(int flag, int projection);
	void setShaderReflection(int flag);
};

void Game::init()
{
	shader = LoadShaderFromMemory(mainvsShaderCode, mainfsShaderCode);
	
	frame = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	
	envTop = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	envBottom = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	envNorth = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	envSouth = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	envEast = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	envWest = LoadRenderTextureWithDepthTexture(FWIDTH, FHEIGHT);
	
	// TODO: TESTING ONLY!
	alphaCt = 1;
	betaCt = 0;
	ghostCt = 0;
	
	player = &players[0];
	*player = Player();
	player->cam.orbit = false;
	//player->cam.projection = CAMERA_PERSPECTIVE;
	
	cam = &player->cam;
	
	map.model = LoadModel("res/mesh/underpass.glb");
	shset(&map.model, shader);
	
	lightCam.orbit = false;
	lightCam.projection = CAMERA_ORTHOGRAPHIC;
	lightCam.fovy = 90;
	
	envCam.orbit = false;
	envCam.projection = CAMERA_PERSPECTIVE;
	envCam.fovy = 90;	
}

void Game::update()
{
	if (IsKeyPressed(ktab))
	{
		cam->orbit = !cam->orbit;
	}
	
	vec2 moused = GetMouseDelta();
	player->cam.rotate(moused.y * DEG2RAD * 0.4, moused.x * DEG2RAD * 0.4, 0);
	
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
	player->update();
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
void Game::present(Camera camera)
{
	for (int i = 0; i < alphaCt + betaCt; i++)
	{
		players[i].render();
	}
	
	BeginMode3D((Camera3D)camera, camera.nearClip, camera.farClip);
	
	DrawCube({0,1,0}, 0.1, 0.1, 0.1, {255,0,255,255});
	DrawModel(map.model, {0,0,0}, 0.2, {255,255,255,255});
	
	EndMode3D();
}

void Game::render()
{
/*	
	// Render depth from light perspective //
	shadow_render:
	
	setShaderDepth(true, CAMERA_ORTHOGRAPHIC);
	
	BeginTextureMode(lightMap);
	present(lightCam);
	EndTextureMode();
	
	// Render environment to cubemap //
	env_render:
	
	setShaderDepth(false, CAMERA_PERSPECTIVE);
	setShaderReflection(false);
	// dont try to render reflections
	// this cubemap has to be rendered first
	
	BeginTextureMode(envTop);
	present(envCam);
	EndTextureMode();
	BeginTextureMode(envBottom);
	present(envCam);
	EndTextureMode();
	BeginTextureMode(envNorth);
	present(envCam);
	EndTextureMode();
	BeginTextureMode(envSouth);
	present(envCam);
	EndTextureMode();
	BeginTextureMode(envEast);
	present(envCam);
	EndTextureMode();
	BeginTextureMode(envWest);
	present(envCam);
	EndTextureMode();
*/	
	main_render:
	
//	setShaderReflection(true);
	
//	BeginTextureMode(frame);
	
	rlEnableDepthTest();
	present(player->cam);
	
//	EndTextureMode();
}