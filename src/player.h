#pragma once

#include "define.h"
#include "input.h"

struct Camera {
	// speed (amount of rotation per frame)
	struct {float x,y;} speed;
	
	// sensitivity (how quickly the camera responds to input)
	float sens;
	
	// smoothing (smooths out quick fluctuations)
	float smooth;
	
	float pitch[3]; // vertical
	float pivot[3]; // horizontal
	float roll[3]; // roll
	
	vec3 pos;
	vec3 lookv;
	
	bool orbit; // true/false
	float orbitDist;
	vec3 orbitOffset;
};

// rotate without smoothing
void cmRotateRaw(Camera* cm, float x, float y)
{
	*cm->pivot += x * cm->speed.x;
	*cm->pivot = fmod(*cm->pivot, 2*M_PI);
	*cm->pitch += y * cm->speed.y;
	*cm->pitch = fmod(*cm->pitch, 2*M_PI);
}

// rotate with built in smoothing
void cmRotate(Camera* cm, float x, float y)
{
	#define interp(rot, delta, speed) \
		rot[2] += delta * speed;\
		rot[2] = fmod(rot[2], 2*M_PI);\
		rot[1] = \
				waverage(rot[1], rot[2], cm->sens);\
		*rot = \
				waverage(*rot, rot[1], cm->smooth);
	
	interp(cm->pivot, x, cm->speed.x)
	interp(cm->pitch, y, cm->speed.y)
	
	#undef interp
}

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