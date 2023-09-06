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

// texture settings
struct Sampler
{
	enum Filter : bool
	{
		NEAREST,
		LINEAR,
	};
	
	enum Wrap : char
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_EDGE,
		CLAMP_BORDER,
	};
	
	Filter minFilter;
	Filter magFilter;
	Filter mipmapFilter;
	
	Filter& min = minFilter;
	Filter& mag = magFilter;
	Filter& mip = mipmapFilter;
	
	Wrap wrapX, wrapY;

	Color edgeColor;
};

enum Type
{
	STREAM, // 1-D
	IMAGE, // 2-D
	VOLUME, // 3-D
	CUBEMAP, // 6 IMAGEs
};

template <Type T>
struct Pixels
{
	uint id;
};

template <>
struct Pixels <IMAGE>
{
	char width;
	char height;
	
	char& w = width;
	char& h = height;
};

// temporary struct to rewrite Texture
struct Tex
{
	uint id;
	
	short width;
	short height;
	short& w = width;
	short& h = height;
	
	Sampler parameters;
	Sampler& param = parameters;
};

void initTextures ();

// load texture from file at path
Texture loadTexture (const char* path);

void drawTexture (Texture tex, float x, float y, float sx, float sy);

// position & size in pixel (screen) coordinates
void drawTexture
(
	Texture tex,
	float2 pos = {0,0}, // bottom left
	float2 size = {0,0}, // use texture size
	bool flip = false
);

void drawTextureFullscreen (Texture tex, bool flip = false, bool flipX = false);

// texture faces camera
void drawBillboard (Texture texture, float x, float y, float z);
