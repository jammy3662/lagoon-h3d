#pragma once

#include "weapon.h"
#include "../engine/camera.h"

struct Player
{
	vec3 position;
	vec3 velocity;
	Camera cam;
	
	float height;
	
	float walkSpeed;
	
	void move(vec3 delta);
	void update();
	void render();
};

void Player::move(vec3 delta)
{
	// TODO: add physics code here
	// to prevent stuff like clipping,
	// gravity, and others
	position = Vector3Add(position, delta);
}
	
void Player::update()
{	
	// update camera state
	cam.anchor = {position.x, position.y + height, position.z};
	cam.refresh();
}

void Player::render()
{
	if (cam.orbit)
	{
		
	}
}

Player newPlayer()
{
	Player ret;

	ret.height = 0.5;
	ret.walkSpeed = 0.05;
	
	ret.position = {0,0,0};
	ret.velocity = {0,0,0};
	
	ret.cam.init();
	ret.cam.refresh();
	
	return ret;
}