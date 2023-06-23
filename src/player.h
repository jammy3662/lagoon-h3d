#pragma once

#include "define.h"
#include "input.h"

struct Camera
{
	// speed (amount of rotation per frame)
	struct {float x, y;} speed;
	
	// sensitivity (how quickly the camera responds to input)
	float sens;
	
	// smoothing (smooths out quick fluctuations)
	float smooth;
	
	float pitch [3]; // vertical
	float pivot [3]; // horizontal
	float roll [3]; // roll
	
	vec3 pos;
	vec3 lookv;
	
	bool orbit; // true/false
	float orbitDist;
	vec3 orbitOffset;
};

// rotate without smoothing
void cmRotateRaw (Camera* cm, float x, float y)
{
	*cm->pivot += x * cm->speed.x;
	*cm->pitch += y * cm->speed.y;
	*cm->pivot = fmod (*cm->pivot, 2 * M_PI);
	*cm->pitch = fmod (*cm->pitch, 2 * M_PI);
}

// rotate with built in smoothing
void cmRotate (Camera* cm, float x, float y)
{
	#define interp(rot, delta, speed) \
		rot [2] += delta * speed; \
		rot [2] = fmod (rot [2], 2 * M_PI); \
		rot [1] = waverage (rot [1], rot [2], cm->sens); \
		*rot = waverage (*rot, rot [1], cm->smooth);
	
	interp (cm->pivot, x, cm->speed.x)
	interp (cm->pitch, y, cm->speed.y)
	
	#undef interp
}

struct Player
{
	let float eye = 0.9;
	let float height = 1;
	
	let float fovMin = 30;
	let float fovMax = 120;
	
	let float orbitDist = height * 1.3;
	
	InputContext * input;
	
	// position
	float x, y, z;
	// movement (position delta)
	float dx, dy, dz;
	
	// movement direction (global)
	// functions as a "queue"
	vec2 wishdir [2];
	
	Camera camera;
	
	float speed; // horizontal speed based on player state
	float accel; // speed interpolation factor (0.9 = swift, 0.1 = sluggish)

	bool onFloor;
	bool onWall;

	bool active;
	bool floored; // standing on solid ground
	bool moving;
	bool sprint;
	bool strafe; // using weapon, move slower
	bool safe; // in your own paint
	bool swim; // in dolphin / whale form
	bool morph; // switching forms
	
	enum Form
	{
		NORMAL, // human form
		MORPH, // morphing (between states)
		SWIM // dolphin / whale form
	}
	form;
	
	void readInput ()
	{
		vec2 wishdir = getAxis1 (input);
		
		if (floored && getButtonNow (input, InputAction::ACCEPT))
			jump ();
	}
	
	void jump()
	{
		
	}
	
	void update()
	{
		readInput();
		movement();
	}
	
	void movement()
	{
		accel = 0.95;
		speed = 1.00;
		
		if (form != Form::MORPH)
		{
			if (sprint) accel -= 0.40;
			if (safe) speed += 0.05;
			if (form == Form::SWIM) speed += 0.20;
		}
	}
};

void updatePlayer (Player * player, InputContext * input)
{
	
}