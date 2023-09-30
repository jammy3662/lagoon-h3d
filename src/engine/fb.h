#pragma once

#include "define.h"
#include "texture.h"

enum Resolution
{
	p360, p540, p720,
	p1080, p2160, p4320,
};

enum BufferOp
{
	ALL = GL_FRAMEBUFFER,
	DRAW = GL_DRAW_FRAMEBUFFER,
	READ = GL_READ_FRAMEBUFFER
};

struct Frame
{
	uint fbo = {};
	Texture outputs [8] = {};
};

extern Frame gbuffer;
extern Frame bufferDefault;

void setRes (Resolution res);
void getRes (float* width, float* height);

// one or more existing textures (any non-cubemap)
Frame genBuffer (Texture* outputs, int nOutputs = 1);

// a single cubemap texture
Frame genBuffer (Texture cubemap);

// one or more color buffers
Frame genBuffer (int width, int height, int nOutputs = 1);

void bindBuffer (Frame buffer, BufferOp mode = ALL);

void bindBuffer (BufferOp mode = ALL); // default fb

// ====================================== //

// set up g-buffer
void initGbuf ();

void drawFrameFullscreen (Frame buf, int attachment);