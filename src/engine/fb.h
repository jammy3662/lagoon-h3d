#pragma once

#include <glad.h>

#include "define.h"

struct Frame
{
	uint fbo;
	
	uint w, h;
};

enum Resolution
{
	p360, p540, p720,
	p1080, p2160, p4320,
};
void setResolution (Resolution res);

// set up g-buffer
void initFramebuffers ();

// get new framebuffer with buffer attachments
// optional flags: non-linear filter defaults to nearest neighbor
Frame newFrame (int width, int height, int attachments, char shrinkLinear = 1, char growLinear = 1);