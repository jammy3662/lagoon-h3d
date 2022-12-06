#pragma once

#include "weapon.h"
#include "../engine/camera.h"

#define PLAYER_EYE (float)0.9
#define PLAYER_HEIGHT 1

#define PLAYER_FOV_MIN 30
#define PLAYER_FOV_MAX 180

#define PLAYER_ORBIT_FOV_ADD 25

#define PLAYER_ORBIT_DISTANCE 2.4
#define PLAYER_ORBIT_HEIGHT_ADD 0

struct Player
{
	vec3 position = {0,0,0};
	vec3 velocity = {0,0,0};
	
	Cam cam;
	float fov = 90;
	
	vec3 lookdir = {0,0,-1};
	
	float walkSpeed = 0.05;
	
	Model model;
	int focused = false;
	
	void init();
	void move(vec3 delta);
	void update(bool readInputs);
	void render();
	
	inline vec3 eye () {
		return {position.x, position.y + PLAYER_EYE, position.z};
	}
	
	float lerpAverage(float from, float to, float weight);
	
	void setOrbit(int o);
};

Player Player_()
{
	Player player;
	
	player.setOrbit(true);
	player.cam.orbitDistance = PLAYER_ORBIT_DISTANCE;
	
	Mesh tempMesh = GenMeshCube(0.5, 1, 0.5);
	player.model = LoadModelFromMesh(tempMesh);
	
	Shader sh = LoadShaderFromMemory(mainvsShaderCode,mainfsShaderCode);
	shbind(sh);
	shset(&player.model);
	
	player.update(false);
	
	return player;
}

float Player::lerpAverage(float from, float to, float weight = 0.5)
{
	return (from * (1-weight)) + (to * (weight));
}

void Player::setOrbit(int o)
{
	cam.orbit = o;
	
	if (o)
	{
		cam.rlcam.fovy = MAX(PLAYER_FOV_MIN, fov - PLAYER_ORBIT_FOV_ADD);
	}
	else
	{
		cam.rlcam.fovy = fov;
	}	
}

void Player::move(vec3 delta)
{
	// TODO: add physics code here
	// to prevent stuff like clipping,
	// gravity, and others
	position = Vector3Add(position, delta);
}

void Player::update(bool readInputs)
{
	if (readInputs)
	{
		// camera / aiming //
		
		vec2 dmouse = GetMouseDelta();
		dmouse.x = -dmouse.x;
		dmouse.y = -dmouse.y;
		cam.rotate(dmouse.y * DEG2RAD, dmouse.x * DEG2RAD, 0);
		
		if (IsKeyPressed(ktab))
		{
			setOrbit(!cam.orbit);
		}
	
		// movement //
		
		vec2 dir = {0,0};
	
		if (IsKeyDown(keast)) dir.x = 1;
		else if (IsKeyDown(kwest)) dir.x = -1;
		
		if (IsKeyDown(knorth)) dir.y = -1;
		else if (IsKeyDown(ksouth)) dir.y = 1;
		
		if (IsKeyDown(kjump))
		{
			move({0, walkSpeed, 0});
		}
		else if (IsKeyDown(kctrl))
		{
			move({0, -walkSpeed, 0});
		}
		
		dir = Vector2Rotate(dir, 2*M_PI - cam.rx);
		dir = Vector2Normalize(dir);
		dir = Vector2Scale(dir, walkSpeed);
		
		move({dir.x, 0, dir.y});
	}
	else
	{
		// allow the camera to continue
		// interpolating even with blocked input
		cam.rotate(0,0,0);
	}
	
	// update camera state
	vec3 eye = position;
	eye.y += PLAYER_EYE;
	
	cam.position = eye;
	cam.update();
}

void Player::render()
{
	if (cam.orbit || !focused)
	{
		DrawModel(model, position, 1, {255,255,255,255});
	}
}