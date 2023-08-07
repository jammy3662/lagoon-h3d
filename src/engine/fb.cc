#include "fb.h"

#include <stdio.h>
#include <glad.h>

#include "shader.h"

// gbuffer resolution
int resx = 1920, resy = 1080;

int framew = resx, frameh = resy, __a, __b;

Frame gbuffer;

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
	
	resx = resolutions [2*res];
	resy = resolutions [2*res + 1];
}

float2 getResolution ()
{
	return {resx, resy};
}

void framebuffer (Frame fb)
{
	framew = fb.width;
	frameh = fb.height;
	glBindFramebuffer (GL_FRAMEBUFFER, fb.fbo);
}

void framebufferDef ()
{
	framew = getResolution().x;
	frameh = getResolution().y;
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

float2 getFrame ()
{
	return {framew, frameh};
}

void initGbuf ()
{
	glGenFramebuffers (1, &gbuffer.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, gbuffer.fbo);
	
	const static int attribCt = 6;
	const static uint formats [] =
		{ GL_RGB, GL_RGB, GL_RGB, GL_RGBA, GL_RG, GL_RG, };
	
	glGenTextures (attribCt, gbuffer.attachments);
	
	for (int i = 0; i < attribCt; i++)
	{
		glBindTexture (GL_TEXTURE_2D, gbuffer.attachments [i]);
		// dont initialize with data as it would be drawn over anyway
		glTexImage2D (GL_TEXTURE_2D, 0, formats [i], resx, resy, 0, formats [i], GL_UNSIGNED_BYTE, 0x0);
		// dont interpolate for downscaling
		// as this tends to look "blurry"
		// and cause a smearing effect
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gbuffer.attachments [i], 0);
	}
	
	glBindTexture (GL_TEXTURE_2D, gbuffer.depthbuf);
	// dont initialize with data as it would be drawn over anyway
	glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resx, resy, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0);
	// dont interpolate for downscaling
	// as this tends to look "blurry"
	// and cause a smearing effect
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthbuf, 0);
	
	// unbind for safety
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

Frame newFramebuf (int w, int h, char shrink, char grow)
{
	Frame ret;
	
	ret.width = w; ret.height = h;
	ret.shrinkFilter = shrink;
	ret.growFilter = grow;
	
	uint minFilter = (shrink) ? GL_LINEAR : GL_NEAREST;
	uint magFilter = (grow) ? GL_LINEAR : GL_NEAREST;
	
	glGenFramebuffers (1, &ret.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, ret.fbo);
	
	glGenTextures (1, ret.attachments);
	
	// default to one color channel
	glBindTexture (GL_TEXTURE_2D, ret.attachments [0]);
	// dont initialize with data as it would be drawn over anyway
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ret.attachments [0], 0);
	
	// depth buffer / texture
	glBindTexture (GL_TEXTURE_2D, ret.depthbuf);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ret.depthbuf, 0);
	
	// unbind for safety
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	
	return ret;
}

Frame cloneFramebuf (Frame fb)
{
	Frame ret;
	
	ret.width = fb.width; ret.height = fb.height;
	ret.shrinkFilter = fb.shrinkFilter;
	ret.growFilter = fb.growFilter;
	
	ret.depthbuf = fb.depthbuf;
	
	uint minFilter = (ret.shrinkFilter) ? GL_LINEAR : GL_NEAREST;
	uint magFilter = (ret.growFilter) ? GL_LINEAR : GL_NEAREST;
	
	glGenFramebuffers (1, &ret.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, ret.fbo);
	
	glGenTextures (1, ret.attachments);
	
	// default to one color channel
	glBindTexture (GL_TEXTURE_2D, ret.attachments [0]);
	// dont initialize with data as it would be drawn over anyway
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, ret.width, ret.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ret.attachments [0], 0);
	
	// depth buffer / texture
	// pixels and parameters already configured, simply bind
	glBindTexture (GL_TEXTURE_2D, ret.depthbuf);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ret.depthbuf, 0);
	
	// unbind for safety
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	
	return ret;
}

void drawFrame (Frame fb, uint attachment, float2 pos, float2 size)
{
	Texture t;
	t.id = fb.attachments [attachment];
	t.w = -fb.width;
	t.h = -fb.height;
	
	// flip image for framebuffer
	drawTexture (t, {-pos.x, -pos.y}, {size.x, -size.y});
}