#pragma once

#include "define.h"

#include <glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>

enum Filter
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	NEAREST_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	NEAREST_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	LINEAR_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	LINEAR_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

enum Wrap
{
	REPEAT = GL_REPEAT,
	MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
	EDGE = GL_CLAMP_TO_EDGE,
	COLOR = GL_CLAMP_TO_BORDER,
};

enum Buffer
{
	EMPTY = 0, // error case
	STREAM = GL_TEXTURE_1D, // 1-D
	IMAGE = GL_TEXTURE_2D, // 2-D
	DEPTHIMG = GL_TEXTURE_DEPTH, // 2-D depth
	CUBEMAP = GL_TEXTURE_CUBE_MAP,
};

// texture settings
struct Sampler
{
	struct
		{ Filter min, mag; }
	filter;
	
	struct
		{ union { Wrap x, s; };
		  union { Wrap y, t; }; }
	wrap;

	Color edgeColor;
};

struct Texture
{
	Buffer type;
	
	unsigned int id; // opengl resource handle
	
	short width, height;
	Sampler* sampler;
};

extern Sampler* const samplerDefault;

void textureInit ();

// image with given width and height
Texture genImage
	(short width, short height, bool depth = false, Sampler* texparam = samplerDefault);
// set of 6 images with given width and height
Texture genCubemap
	(short width, short height, Sampler* texparam = samplerDefault);

// load texture from file at path
Texture loadTexture
	(const char* path, Sampler* texparam = samplerDefault);
	
// position & size in pixel (screen) coordinates
void drawTexture
	(Texture tex, int x = 0, int y = 0, int w = 0, int h = 0);

void drawFullscreen
	(Texture tex, bool flipV = false, bool flipH = false);
