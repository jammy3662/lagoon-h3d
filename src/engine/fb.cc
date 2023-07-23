#include "fb.h"

#include <stdio.h>

// internal render resolution
uint framew = 1920, frameh = 1080;

struct gbuffer
{
	uint fbo;
}
gbuffer;

const int resolutions [] =
{
	640, 360,
	960, 540,
	1280, 720,
	1920, 1080,
	3840, 2160,
	7680, 4320,
};

void setResolution (Resolution res)
{
	if ((uint)res > p4320)
	{
		fprintf (stderr, "Setting invalid resolution (%u)\n", res);
	}
	
	framew = resolutions [2*res];
	frameh = resolutions [2*res + 1];
}

void initFramebuffers ()
{
	glGenFramebuffers (1, &gbuffer.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, gbuffer.fbo);
}

Frame newFrame (int w, int h, char shrink, char grow)
{
	Frame ret;
	
	glGenFramebuffers (1, &ret.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, ret.fbo);
	
	return ret;
}