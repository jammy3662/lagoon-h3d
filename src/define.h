#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

#include "../include/ext.h"

#define let constexpr static

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
