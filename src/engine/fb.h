#pragma once

#include "define.h"
#include "texture.h"

struct Frame
{
	uint fbo;
	uint depthbuf;
	
	int width, height;
	bool shrinkLinear, expandLinear;
	
	uint attachments [8];
};

extern Frame gbuffer;

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
void framebuffer();

// current framebuffer dimensions
float2 getFrame ();

// set up g-buffer
void initGbuf ();

// bind and render to gbuffer
void backbuffer ();

// get new framebuffer
// optional flags: non-linear filter defaults to nearest neighbor
Frame newFramebuf (int width, int height, bool shrinkLinear = 0, bool expandLinear = 1);

// get new framebuffer, share existing depth buffer and texture parameters
// (sizes MUST match, else undefined behavior)
Frame cloneFramebuf (Frame buf);

// should flip vertically only when rendering directly to screen
/* (UNSTABLE - DO NOT USE)
void drawFrame (Frame buf, uint attachment, bool flip = false, float2 pos = {0,0}, float2 size = {0,0});
*/

void drawFrameFullscreen (Frame buf, int attachment);