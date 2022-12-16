#pragma once

#include "weapon.h"
#include "../engine/camera.h"

#define PLAYER_EYE (float)0.9
#define PLAYER_HEIGHT 1

#define PLAYER_FOV_MIN 30
#define PLAYER_FOV_MAX 180

#define PLAYER_ORBIT_FOV_ADD 5

#define PLAYER_ORBIT_DISTANCE 1.5

struct Player {

static RenderTexture frame;

vec3 position = {0,0,0};
vec3 velocity = {0,0,0};

vec2 dir = {0,0}; // movement direction (global)
vec2 dirNext = {0,0};
vec2 dirTarget = {0,0};

Cam cam;
float fov = 90;

float walkSpeed = 0.05;

Model model;
float opacity = 1;

int idle = true;
int fast = false;

int ceta = false;
int shift = false; // true when in between forms

inline int isWalking()  { return !idle && !ceta && !fast; }
inline int isRunning()  { return !idle && !ceta && fast; }
inline int isSliding()  { return !idle && ceta && !fast; }
inline int isSwimming() { return !idle && ceta && fast; }

inline
Camera3D& camera()
{
	return cam;
}

inline
vec3& lookdir()
{
	return cam.lookdir;
}

void init();

void move(vec3 delta)
{
	// TODO: add physics code here
	// to prevent stuff like clipping,
	// gravity, and others
	position = Vector3Add(position, delta);
}

void update(bool readInputs)
{
	vec2 dmouse = {0,0};
	vec2 wishdir = {0,0};
	
	if (readInputs)
	{
		// camera / aiming //
		dmouse = GetMouseDelta();
		dmouse.x = -dmouse.x;
		dmouse.y = -dmouse.y;
		
		if (IsKeyPressed(ktab))
		{
			setOrbit(!cam.orbit);
			
		}
		
		// movement //
		if (IsKeyDown(keast)) wishdir.x = 1;
		else if (IsKeyDown(kwest)) wishdir.x = -1;
		
		if (IsKeyDown(knorth)) wishdir.y = -1;
		else if (IsKeyDown(ksouth)) wishdir.y = 1;
		
		wishdir = Vector2Rotate(wishdir, 2*M_PI - cam.rx);
		wishdir = Vector2Normalize(wishdir);
		
		if (IsKeyDown(kjump))
		{
			move({0, walkSpeed, 0});
		}
		else if (IsKeyDown(kctrl))
		{
			move({0, -walkSpeed, 0});
		}
	}
	
	// interpolate movement //
	dirTarget.x = waverage(dirTarget.x, wishdir.x, 0.3);
	dirTarget.y = waverage(dirTarget.y, wishdir.y, 0.3);

	dirNext.x = waverage(dirNext.x, dirTarget.x, 0.2);
	dirNext.y = waverage(dirNext.y, dirTarget.y, 0.2);

	dir.x = waverage(dir.x, dirNext.x, 0.9);
	dir.y = waverage(dir.y, dirNext.y, 0.9);

	// TEST
	dir = Vector2Scale(dir, walkSpeed);

	move({dir.x, 0, dir.y});
	
	// camera //
	cam.rotate(dmouse.y * DEG2RAD, dmouse.x * DEG2RAD, 0);
	
	// interpolate between povs
	const float povsp = 0.5;
	if (!cam.orbit)
	{
		cam.fovy = waverage(cam.fovy, MAX(PLAYER_FOV_MIN, fov - PLAYER_ORBIT_FOV_ADD), povsp);
		cam.orbitDistance =
			waverage(cam.orbitDistance, 0, povsp);
	}
	else
	{
		cam.fovy = waverage(cam.fovy, fov, povsp);
		cam.orbitDistance = 
			waverage(cam.orbitDistance, PLAYER_ORBIT_DISTANCE, povsp);
	}
	
	opacity = cam.ry / (M_PI);
	opacity += 0.5;
	opacity = 0;
	
	// update camera state //
	cam.anchor = eye();
	cam.update();
}

// set `forceDraw` to make player
// draw even in first person cam
void present(int forceDraw = 0)
{
	if (cam.orbit || forceDraw)
	{
		vec3 point = position;
		point.y += PLAYER_HEIGHT * 0.5;
		DrawModel(model, point, 1, {255,255,255,255});
	}
}

inline vec3 eye () { return {position.x, position.y + PLAYER_EYE, position.z}; }

void setOrbit(int o)
{
	cam.orbit = o;
}

}; // Player //

RenderTexture Player::frame;

Player Player_()
{
	Player player;
	
	SetCameraMode(player.cam, CAMERA_CUSTOM);
	player.cam.up = {0,1,0};
	player.cam.fovy = 90;
	player.cam.projection = CAMERA_PERSPECTIVE;
	
	player.setOrbit(true);
	player.cam.orbitDistance = PLAYER_ORBIT_DISTANCE;
	player.cam.orbitOffset = {
		PLAYER_ORBIT_DISTANCE * 0,
		PLAYER_ORBIT_DISTANCE * 0.22,
		PLAYER_ORBIT_DISTANCE * 0};
	
	Mesh tempMesh = GenMeshCube (
		PLAYER_HEIGHT*0.5,
		PLAYER_HEIGHT,
		PLAYER_HEIGHT*0.5
	);
	player.model = LoadModelFromMesh(tempMesh);
	
	Shader sh = LoadShaderFromMemory(mainvsShaderCode,mainfsShaderCode);
	
	player.update(false);
	
	return player;
}
