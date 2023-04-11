#pragma once

#include "engine.h"
//#include "weapon.h"
#include "camera.h"

#define PLAYER_EYE (float)0.9
#define PLAYER_HEIGHT 1

#define PLAYER_FOV_MIN 30
#define PLAYER_FOV_MAX 180

#define PLAYER_ORBIT_FOV_ADD 15

#define PLAYER_ORBIT_DISTANCE (PLAYER_HEIGHT * 1.3)

using namespace Input;

struct Player
{
	const float eye = 0.9;
	const float height = 1;
	
	const float orbitDist = height *	1.3;
	const int fovMin = 30;
	const int fovMax = 180;
	
	vec3 pos; // position
	vec3 vel; // velocity
	
	// movement direction (global)
	// functions as a "queue"
	vec2 wishdir[2];
	
	Camera camera;
	
	float speed; // horizontal speed based on player state
	float accel; // speed interpolation factor (0.9 = swift, 0.1 = sluggish)

	bool onFloor;
	bool onWall;

	enum State {
		FLOORED = 0x0, // standing on solid ground
		MOVING =	0x1,
		SPRINT = 0x2,
		STRAFE = 0x4, // slower movement
		SAFE = 0x8, // true when submerged in paint
		SWIM = 0x10, // true in dolphin form
		MORPH = 0x20, // true when in between forms
		ACTIVE =	0x40, // unset externally for pausing
	};
	int state_;
	
	inline bool state( State _state_ ) {
		return (state_ & _state_);
	}
	
	inline void setstate( State _state_, int flag) {
		if (flag) {
			state_ |= _state_;
		} else {
			state_ &= ~_state_;
		}
	}
	
	void handleInput()
	{
		vec2 wishdir = Input::joystick;
		
		if (Input::isPressed(Input::ACCEPT)
				&& state(FLOORED)) {
			jump();
		}
	}
	
	void jump()
	{
		
	}
	
	void update()
	{
		handleInput();
		physicsCheck(onFloor, onWall);
		movement();
		
		if (Input::isPressed(Input::ACCEPT)) {
			if (state(FLOORED)) jump();
		}
	}
	
	void physicsCheck(bool& isOnFloor, bool& isOnWall)
	{
		
	}
	
	void movement()
	{
		accel = 0.95;
		speed = 1.00;
		
		if (!state(MORPH))
		{
			if (state(SPRINT)) accel -= 0.40;
			if (state(SAFE)) speed += 0.05;
			if (state(SWIM)) speed += 0.20;
		}
	}
};