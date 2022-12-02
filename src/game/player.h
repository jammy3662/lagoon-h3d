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
	
	Player()
	{
		cam = Camera();
		
		height = 0.5;
		walkSpeed = 0.05;

		position = {0,0,0};
		velocity = {0,0,0};

		cam.refresh();
	}
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
	DrawRectangle(100,100,100,100,{100,0,100,255});
	if (cam.orbit)
	{
		
	}
}