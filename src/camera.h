#pragma once

#include "engine.h"

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