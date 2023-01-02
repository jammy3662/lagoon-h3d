#pragma once

#include "../compile/SHADER.c"
#include "../compile/TEXTURE.c"

#include "../engine/define.h"
#include "../engine/shader.h"

/*
struct RasterPass
{
	RenderTexture* frame = 0;
	~RasterPass()
	{
		EndMode3D();
		// always validate here because null
		// means using the default framebuffer
		if (frame) EndTextureMode();
	}
};

typedef enum // RasterClear
{
	CLEAR_NONE = GL_NONE,
	CLEAR_COLOR = GL_COLOR_BUFFER_BIT,
	CLEAR_DEPTH = GL_DEPTH_BUFFER_BIT,
	CLEAR_ALL = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
	// (raylib doesnt support stencil buffer out of the box)
} RasterClear;

RasterPass Raster(ShaderPass shader, Camera3D camera, RenderTexture* frame, RasterClear clearType, vec4 clearColor)
{
	// always validate here because null
	// means to SKIP binding the framebuffer
	if (frame) BeginTextureMode(*frame);
	
	SHADER.use(shader.shader);
	shader.bind();
	
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(clearType);
	
	Begin3D(camera, frame->texture.width, frame->texture.height);
	
	return (RasterPass){.frame = frame};
}

// preserves current shader and bindings
RasterPass Raster(Camera3D camera, RenderTexture* frame, RasterClear clearType, vec4 clearColor)
{
	// always validate here because null
	// means to SKIP binding the framebuffer
	if (frame) BeginTextureMode(*frame);
	
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(clearType);
	
	Begin3D(camera, frame->texture.width, frame->texture.height);
	
	return (RasterPass){.frame = frame};
}
*/