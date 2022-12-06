#pragma once

#include "weapon.h"
#include "../engine/camera.h"

#define PLAYER_EYE 0.9
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
	
	float lerpAverage(float from, float to, float weight);
	
	void setOrbit(int o);
};

Player Player_()
{
	Player player;
	
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
/*
		dmouse.x *= DEG2RAD * sensX * -1;
		dmouse.y *= DEG2RAD * sensY;
		
		// DEBUG //
		if (IsKeyPressed(kup))
		{
			cSensX = MIN(cSensX + 5, 100);
			cSensY = MIN(cSensY + 5, 100);
			printf("sens: %i\n", cSensX); 
		}
		else if (IsKeyPressed(kdown))
		{
			cSensX = MAX(cSensX - 5, 0);
			cSensY = MAX(cSensY - 5, 0);
			printf("sens: %i\n", cSensX); 
		}
		
		// DEBUG //
		if (IsKeyPressed(kleft))
		{
			cSmoothX = MAX(cSmoothX - 5, 0);
			cSmoothY = MAX(cSmoothY - 5, 0);
			printf("smooth: %i\n", cSmoothX); 
		}
		else if (IsKeyPressed(kright))
		{
			cSmoothX = MIN(cSmoothX + 5, 100);
			cSmoothY = MIN(cSmoothY + 5, 100);
			printf("smooth: %i\n", cSmoothX); 
		}
		
		float dwx = (float)CLAMP(1, (cSensX), 100) * 0.01;
		float dwy = (float)CLAMP(1, (cSensY), 100) * 0.01;
		
		float cwx = (float)CLAMP(1, (100 - cSmoothX), 100) * 0.01;
		float cwy = (float)CLAMP(1, (100 - cSmoothY), 100) * 0.01;
		
		drx = lerpAverage(drx, dmouse.x, dwx);
		dry = lerpAverage(dry, dmouse.y, dwy);
		
		crx = lerpAverage(crx, drx, cwx);
		cry = lerpAverage(cry, dry, cwy);
		
		ry = CLAMP((float)-1.55, ry + cry, (float)1.55);
		rx = fmod(rx + crx, 2*M_PI);
		
		if (IsKeyPressed(ktab))
		{
			setOrbit(!orbit);
		}
*/
	
		//
		// movement input
		//
		
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

	vec2 dmouse = GetMouseDelta();
	dmouse.x = -dmouse.x;
	dmouse.y = -dmouse.y;
	cam.rotate(dmouse.y * DEG2RAD, dmouse.x * DEG2RAD, 0);
	
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