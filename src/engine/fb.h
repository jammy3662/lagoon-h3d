#pragma once

#include "define.h"
#include "texture.h"

struct Frame
{
	uint fbo;
	uint depthbuf;
	
	int width, height;
	char shrinkFilter, growFilter;
	
	uint attachments [8];
};

enum Resolution
{
	p360, p540, p720,
	p1080, p2160, p4320,
};
void setResolution (Resolution res);

float2 getResolution ();

// bind a framebuffer (drawing and reading)
void framebuffer (Frame buf);

// bind default framebuffer
void framebufferDef();

// current framebuffer dimensions
float2 getFrame ();

// set up g-buffer
void initGbuf ();

// get new framebuffer
// optional flags: non-linear filter defaults to nearest neighbor
Frame newFramebuf (int width, int height, char shrinkLinear = 0, char growLinear = 1);

// get new framebuffer, share existing depth buffer and filtering options
// (sizes MUST match, else undefined behavior)
Frame cloneFramebuf (Frame buf);

void drawFrame (Frame buf, uint attachment, float2 pos = {0,0}, float2 size = {0,0});