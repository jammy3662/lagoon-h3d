#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include <horde3d/Horde3D.h>
#include <horde3d/Horde3DUtils.h>

#include "../include/ext.h"

// custom (more crude) bool type
#define bool char
#define false 0
#define true 1

typedef struct{double x,y;} vec2;
typedef struct{double x,y,z;} vec3;
typedef struct{double x,y,z,w;} vec4;

float waverage(float from, float to, float weight) {
	return (from * (1-weight)) + (to * (weight));
}

vec2 waverage(vec2 from, vec2 to, float weight) {
	return vec2 {
		waverage(from.x, to.x, weight),
		waverage(from.y, to.y, weight)
	};
}

