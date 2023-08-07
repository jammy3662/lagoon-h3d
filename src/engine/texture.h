#pragma once

#include "define.h"

#include <glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>

/*
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
*/

struct Texture
{
	uint id;
	int w, h;
};

void initTextures ();

// load texture from file at path
Texture loadTexture (const char* path);

// position and size are in pixel coordinates
// size of 0 uses original texture size
void drawTexture (
	Texture texture,
	float2 pos = {0,0}, // top left
	float2 size = {0,0}
);

// texture faces camera
void drawBillboard (Texture texture, float x, float y, float z);
