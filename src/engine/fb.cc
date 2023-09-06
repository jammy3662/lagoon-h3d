#include "fb.h"

#include <stdio.h>
#include <glad.h>

#include "shader.h"

const char* gbufVert =
"#version 330 core\n"

"layout (location = 0) in vec3 Pos;"
"layout (location = 1) in vec3 Normal;"
"layout (location = 2) in vec4 Color;"
"layout (location = 3) in vec2 Uv0;"
"layout (location = 4) in vec2 Uv1;"
"layout (location = 5) in vec3 Tangent;"

"uniform sampler2D albedo;"
"uniform sampler2D heightmap;"

"uniform mat4 mvp;"
"uniform mat4 modelMat;"
"uniform mat4 normalMat;"
"uniform mat4 sunMat;"

"out vec3 pos;"
"out vec3 normal;"
"out vec4 color;"
"out vec2 uv0;"
"out vec2 uv1;"
"out vec3 tangent;"
"out vec4 shadowpos;"

"void main ()"
"{"
"	pos = (mvp * vec4 (Pos, 1.0)).xyz;"
"	normal = (normalMat * vec4 (Normal, 1.0)).xyz;"
"	color = Color;"
"	uv0 = Uv0;"
"	uv1 = Uv1;"
"	tangent = Tangent;"

"	gl_Position = mvp * vec4 (pos, 1.0);"
"}"
;

const char* gbufFrag =
"#version 330 core\n"

"in vec3 pos;"
"in vec3 normal;"
"in vec4 color;"
"in vec2 uv0;"
"in vec2 uv1;"
"in vec3 tangent;"
"in vec4 shadowpos;"

"out vec3 POS;"
"out vec3 NORMAL;"
"out vec4 COLOR;"
"out vec2 UV0;"
"out vec2 UV1;"
"out vec3 TANGENT;"
"out vec4 ALBEDO;"
"out vec4 SHADOWPOS;"

"void main ()"
"{"
"	POS = pos;"
"	NORMAL = normal;"
"	COLOR = color;"
"	UV0 = uv0;"
"	UV1 = uv1;"
"	TANGENT = tangent;"
// TODO: lookup texture and calulate light space position
"}"
;

// gbuffer resolution
int resx = 1920, resy = 1080;

int framew = resx, frameh = resy;

Frame gbuffer;
Shader gbufferShader;

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

void framebuffer ()
{
	framew = resx;
	frameh = resy;
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

float2 getFrame ()
{
	return {framew, frameh};
}

// needs to be called again
// to sync resolution, if it changes
void initGbuf ()
{
	gbuffer.width = resx;
	gbuffer.height = resy;
	
	if (!gbufferShader.compiled)
		gbufferShader = loadShaderCode (gbufVert, gbufFrag);
	
	glGenFramebuffers (1, &gbuffer.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, gbuffer.fbo);
	
	const static uint buffers [] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
	};
	
	glDrawBuffers (3, buffers);
	
	const static uint attribs [] =
	{
		GL_RGBA, // color + opacity
		GL_RGB, // normal
		GL_RGB, // position
	};
	
	constexpr int attribCt = sizeof (attribs) / sizeof (uint);
	
	for (int i = 0; i < attribCt; i++)
	{
		glGenTextures (1, gbuffer.attachments + i);
		glBindTexture (GL_TEXTURE_2D, gbuffer.attachments [i]);
		// dont initialize with data as it would be drawn over anyway
		glTexImage2D (GL_TEXTURE_2D, 0, attribs [i], resx, resy, 0, attribs [i], GL_UNSIGNED_BYTE, 0x0);
		// dont interpolate for downscaling -
		// tends to look "blurry" or "smeared."
		// no mipmaps, its being drawn as a fullscreen quad
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gbuffer.attachments [i], 0);
	}
	
	// same texture parameters as the other attachments
	glBindTexture (GL_TEXTURE_2D, gbuffer.depthbuf);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resx, resy, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbuffer.depthbuf, 0);
	glDrawBuffer (GL_DEPTH_ATTACHMENT);
	
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

void backbuffer ()
{
	framebuffer (gbuffer);
	
	glUseProgram (gbufferShader.id);
}

Frame newFramebuf (int w, int h, bool shrink, bool grow)
{
	Frame ret;
	
	ret.width = w; ret.height = h;
	ret.shrinkLinear = shrink;
	ret.expandLinear = grow;
	
	uint minFilter = (shrink) ? GL_LINEAR : GL_NEAREST;
	uint magFilter = (grow) ? GL_LINEAR : GL_NEAREST;
	
	glGenFramebuffers (1, &ret.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, ret.fbo);
	
	glDrawBuffer (GL_COLOR_ATTACHMENT0);
	
	glGenTextures (1, ret.attachments);
	
	glBindTexture (GL_TEXTURE_2D, ret.attachments [0]);
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
	// a shallow copy is fine here
	// the resource handle will be replaced with the new fbo
	Frame ret = fb;
	
	uint minFilter = (ret.shrinkLinear) ? GL_LINEAR : GL_NEAREST;
	uint magFilter = (ret.expandLinear) ? GL_LINEAR : GL_NEAREST;
	
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
	
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	
	return ret;
}

/*
void drawFrame (Frame buf, uint attachment, bool flip, float2 pos, float2 size)
{
	Texture tex;
	tex.id = buf.attachments [attachment];
	tex.w = buf.width; tex.h = buf.height;
	
	drawTexture (tex, pos, size, flip);
}
*/

void drawFrameFullscreen (Frame buf, int attachment)
{
	Texture tex;
	tex.id = buf.attachments [attachment];
	
	//drawTextureFullscreen (tex, true, true);
	drawTextureFullscreen (tex, true, false);
}