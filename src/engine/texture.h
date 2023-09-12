#pragma once

#include "define.h"

#include <glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>

// texture settings
struct Sampler
{
	enum Filter : char
		{
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			NEAREST_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			LINEAR_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		};
	
	enum Wrap : char
		{
			REPEAT = GL_REPEAT,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			EDGE = GL_CLAMP_TO_EDGE,
			COLOR = GL_CLAMP_TO_BORDER,
		};
	
	struct
		{	
			Filter min, mag;
		}
	filter;
	
	struct
		{
			union { Wrap x, s; };
			union { Wrap y, t; };
		}
	wrap;

	Color edgeColor;
};
typedef Sampler::Filter Filter;
typedef Sampler::Wrap Wrap;

Sampler genSampler
(
	Filter min = Filter::LINEAR, Filter mag = Filter::NEAREST,
	Wrap x = Wrap::MIRRORED_REPEAT, Wrap y = Wrap::MIRRORED_REPEAT,
	Color edgeColor = {0,0,0,0}
);

struct Texture
{
	enum Buffer
		{
			EMPTY = 0, // error case
			STREAM, // 1-D
			IMAGE, // 2-D
			DEPTH, // 2-D depth
			CUBEMAP,
		}
	type;
	
	uint id; // opengl resource handle
	
	short width, height;
	Sampler sampler;
};

// 1-dimensional pixel buffer, rarely used
Texture genStream (short length, Sampler params);
// image with given width and height
Texture genImage (short width, short height, Sampler params, bool depth = false);
// set of 6 images with given width and height
Texture genCubemap (short width, short height, Sampler params);

// ==========

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
