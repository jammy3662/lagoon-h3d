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

static const uint glAttachments [] =
{
	GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7,
};

// gbuffer resolution
struct
{
	int x = 1920, y = 1080;
}
res;

struct
{
	int w = res.x, h = res.y;
}
frame;

const int resolutions [] =
{
	640, 360,
	960, 540,
	1280, 720,
	1920, 1080,
	3840, 2160,
	7680, 4320,
};

void setRes (Resolution r)
{
	res.x = resolutions [2 * r];
	res.y = resolutions [2 * r + 1];
}

void getRes (float* w, float* h)
{
	*w = res.x; *h = res.y;
}

Frame gbuffer;
Shader gbufferShader;

Frame bufferDefault;

Frame genBuffer (Texture* outs, int nOutput)
{
	Frame ret;
	
	glGenFramebuffers (1, &ret.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, ret.fbo);
	
	for (int i = 0; i < nOutput; ++i)
	{
		Texture t = outs[i];
		ret.outputs [i] = t;
		
		switch (t.type)
{
		case STREAM:
		break;
		// unsupported (so far)
		// TODO: support this, maybe
		
		case IMAGE:
			glBindTexture (GL_TEXTURE_2D, t.id);
			glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t.id, 0);
			glBindTexture (GL_TEXTURE_2D, 0);
		break;
		
		case DEPTHIMG:
			glBindTexture (GL_TEXTURE_2D, t.id);
			glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, t.id, 0);
			glBindTexture (GL_TEXTURE_2D, 0);
		break;
		
		case CUBEMAP:
			fprintf (stderr, "");
		break;
		
		default:
			fprintf (stderr, "[x] Attaching generic (uninitialized) texture to framebuffer\n");
		break;
}
	}
	
	return ret;
}

Frame genBuffer (Texture t)
{
	Frame ret;
	
	if (t.type != CUBEMAP)
	{
		ret.fbo = {};
		fprintf (stderr, "[x] Trying to attach non-cubemap texture as cubemap\n");
		return ret;
	}
	
	ret.outputs [0] = t;
	
	glGenFramebuffers (1, &ret.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, ret.fbo);
	
	glBindTexture (GL_TEXTURE_CUBE_MAP, t.id);
	for (int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X +	i, t.id, 0);
	}
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	
	return ret;
}

Frame genBuffer (int w, int h, int outs)
{
	Frame ret;
	
	glGenFramebuffers (1, &ret.fbo);
	glBindFramebuffer (GL_FRAMEBUFFER, ret.fbo);
	
	glDrawBuffers (outs, glAttachments);
	
	for (int i = 0; i < outs; ++i)
	{
		Texture t = genImage (w, h);
		ret.outputs [i] = t;
		glBindTexture (GL_TEXTURE_2D, t.id);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t.id, 0);
		glBindTexture (GL_TEXTURE_2D, 0);
	}
	
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	
	return ret;
}

void bindBuffer (Frame buffer, BufferOp mode)
{
	glBindFramebuffer (mode, buffer.fbo);
}

void bindBuffer (BufferOp mode)
{
	glBindFramebuffer (mode, 0);
}

// needs to be called again to sync resolution, if it changes
void initGbuf ()

{
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
}