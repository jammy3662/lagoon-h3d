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

// set up g-buffer
void initGbuf ();

// set the render target for proceeding draw calls
void targetBuf (Frame framebuffer);

// render to main framebuffer
void targetBufDefault ();

// get new framebuffer
// optional flags: non-linear filter defaults to nearest neighbor
Frame newFramebuf (int width, int height, char shrinkLinear = 0, char growLinear = 1);

// get new framebuffer, share existing depth buffer and filtering options
// (sizes MUST match, else undefined behavior)
// optional flags: non-linear filter defaults to nearest neighbor
Frame cloneFramebuf (Frame framebuffer);

// gets a texture from the framebuffer's specified output buffer
// (UNSAFE - not bounds checked)
Texture fbAttachment (Frame framebuffer, uint attachment);