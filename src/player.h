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

struct Player
{
	vec3 pos; // position
	vec3 vel; // velocity
	
	// movement direction (global)
	// functions as a "queue"
	vec2 wishdir[3];
	
	Camera camera;

	enum State
	{
		NONE = 0,
		
		MOVING =	0x1,
		SPRINT =	0x2,
		
		CETA =	0x4,
		SHIFT =	0x8, // true when in between forms
		
		ACTIVE =	0x10, // unset externally for pausing
	}
	state;
};

// reads input bindings from arguments
void plReadInput(Player* pl, int north, int east, int confirm, int shift, int ctrl, int tab)
{
	float wishdir[2] = {0.0, 0.0};
	
	if (east > 0) wishdir[0] = 1;
	else if (east < 0) wishdir[0] = -1;
	
	if (north > 0) wishdir[1] = 1;
	else if (north < 0) wishdir[1] = -1;
	
	// TODO: normalize wishdir here
	// or replace the whole thing above with
	// hardcoded angles as lookup (probably better)
	
	if (confirm) pl->vel.y = 1;
	else if (ctrl) pl->vel.y = -1;
	
	if (tab) pl->camera.orbit = false;
}

void plUpdate(Player* pl)
{
	if (pl->state & Player::State::ACTIVE)
	{
		// TODO: read input from backend lib
		// and pass in here
		plReadInput(pl, 0, 0, 0, 0, 0, 0);
	}
	
	// interpolate movement
	// using weighted average for smoothing;
	// there is probably a better way
	pl->wishdir[2] = waverage(
			pl->wishdir[2], pl->wishdir[1], 0.3);
	pl->wishdir[1] = waverage(
			pl->wishdir[1], pl->wishdir[2], 0.2);
	
	*pl->wishdir = waverage(
			pl->wishdir[0], pl->wishdir[1], 0.9);
	
	// interpolate pov
	// TODO: implement this with an
	// animation engine rather than
	// crude interpolation
}
