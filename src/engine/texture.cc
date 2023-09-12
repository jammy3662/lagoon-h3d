#include "texture.h"
#include "fb.h"
#include "shader.h"
#include "disk.h"

#include <stdio.h>

#include <stb_image.h>

const char* textureShaderV =
"#version 330 core\n"

"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec2 uv;"

"uniform sampler2D tex;"

"uniform mat4 transform;"

"out vec2 UV;"

"void main ()"
"{"
"	gl_Position = transform * vec4 (pos, 1.0);"
"	UV = uv;"
"}"
;

const char* textureShaderF =
"#version 330 core\n"

"in vec2 UV;"

"uniform sampler2D tex;"

"out vec4 fragColor;"

"void main ()"
"{"
"	fragColor = texture (tex, UV);"
"}"
;

const float textureQuad [] =
{
	-1, 1, 0, 0, 0,		1, 1, 0, 1, 0,
	-1, -1, 0, 0, 1,		1, -1, 0, 1, 1,
};
enum {TL, TR, BL, BR};
const uint textureQuadIndices [] =
{
	TR, BR, TL,
	BR, BL, TL,
};

Shader textureShader;
uint textureVao, textureLoc, transformLoc;

void initTextures ()
{
	textureShader = loadShaderCode (textureShaderV, textureShaderF);
	
	textureLoc = glGetUniformLocation (textureShader.id, "tex");
	transformLoc = glGetUniformLocation (textureShader.id, "transform");
	
	if (textureShader.compiled)
		printf ("[.] Compiled texture shader\n");
		
	uint vao, vbo, ebo;
	
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (textureQuad), textureQuad, GL_STATIC_DRAW);
	
	// position
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)0);
	glEnableVertexAttribArray (0);
	// uv
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*) (3 * sizeof (float)));
	glEnableVertexAttribArray (1);
	
	glGenBuffers (1, &ebo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(textureQuadIndices), textureQuadIndices, GL_STATIC_DRAW);
	
	glBindVertexArray (0);
	
	textureVao = vao;
}

Sampler genSampler (Filter min, Filter mag, Wrap x, Wrap y, Color border)
{
	Sampler ret;
	
	// just populate the object
	// texture functions will set hardware state
	
	ret.filter.min = min;
	ret.filter.mag = mag;
	
	ret.wrap.x = x;
	ret.wrap.y = y;
	
	ret.edgeColor = border;
	
	return ret;
}

Texture genStream (short length, Sampler sampler)
{
	Texture ret;
	ret.type = Texture::STREAM;
	ret.width = length; ret.height = 0;
	ret.sampler = sampler;
	
	glGenTextures (1, &ret.id);
	glBindTexture (GL_TEXTURE_1D, ret.id);
	
	glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, sampler.filter.min);
	glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, sampler.filter.mag);
	
	glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, sampler.wrap.s);
	
	if (sampler.wrap.x == Wrap::COLOR)
	{
		int color = sampler.edgeColor.a |
						(sampler.edgeColor.b << 1) |
						(sampler.edgeColor.g << 2) |
						(sampler.edgeColor.r << 3);
		glTexParameteri (GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, color);
	}
	
	glTexImage1D (GL_TEXTURE_1D, 0, GL_RGBA, length, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0);
	
	return ret;
}

Texture genImage (short width, short height, Sampler sampler, bool depth)
{
	Texture ret;
	ret.type = (!depth) ? Texture::IMAGE : Texture::DEPTH;
	ret.width = width; ret.height = height;
	ret.sampler = sampler;
	
	glGenTextures (1, &ret.id);
	glBindTexture (GL_TEXTURE_2D, ret.id);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.filter.min);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.filter.mag);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrap.s);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrap.t);
	
	if (sampler.wrap.x == Wrap::COLOR ||
		sampler.wrap.y == Wrap::COLOR)
	{
		int color = sampler.edgeColor.a |
						(sampler.edgeColor.b << 1) |
						(sampler.edgeColor.g << 2) |
						(sampler.edgeColor.r << 3);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	}
	
	if (!depth)
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0);
	else
		glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0);
	
	return ret;
}

Texture genCubemap (short width, short height, Sampler sampler)
{
	Texture ret;
	ret.type = Texture::CUBEMAP;
	ret.width = width; ret.height = height;
	ret.sampler = sampler;
	
	glGenTextures (1, &ret.id);
	glBindTexture (GL_TEXTURE_2D, ret.id);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.filter.min);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.filter.mag);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrap.s);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrap.t);
	
	if (sampler.wrap.x == Wrap::COLOR ||
		sampler.wrap.y == Wrap::COLOR)
	{
		int color = sampler.edgeColor.a |
						(sampler.edgeColor.b << 1) |
						(sampler.edgeColor.g << 2) |
						(sampler.edgeColor.r << 3);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	}
	
	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0);
	}
	
	return ret;
}

Texture uploadTexture (int w, int h, int channels, char* texels)
{
	Texture ret;
	
	ret.width = w;
	ret.height = h;

	glGenTextures (1, &ret.id);
	glBindTexture (GL_TEXTURE_2D, ret.id);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// linear for shrinking, interpolate mipmap level
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	// nearest for expanding, interpolate mipmap level
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (channels > 5)
	{
		printf ("[x] Too many channels in image: %u\n", channels);
		channels = 1;
	}
	
	// 0 element for padding (1 channel = GL_R)
	int fmt [] = {GL_R, GL_R, GL_RG, GL_RGB, GL_RGBA};
	
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h,
		0, fmt [channels], GL_UNSIGNED_BYTE, texels);
	glGenerateMipmap (GL_TEXTURE_2D);

	return ret;
}

Texture loadTexture (const char* path)
{
	Texture ret;
	
	// TODO:	load file using
	// resource manager
	
	int w, h, channels;
	char* texels = (char*) stbi_load (path, &w, &h, &channels, 0);
	
	uint fmt;
	switch (channels)
	{
		case (1): fmt = GL_R; break;
		case (2): fmt = GL_RG; break;
		case (3): fmt = GL_RGB; break;
		case (4): fmt = GL_RGBA; break;
		
		default:
			fprintf (stderr, "[x] Texture '%s' not found\n", path);
	}
	
	if (texels)
	{
		printf ("[.] Texture '%s' uploaded\n", path);
		
		Texture t = genImage (w, h, genSampler());
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, fmt, GL_UNSIGNED_BYTE, texels);
		
		return uploadTexture (w, h, channels, texels);
	}
	
	return ret;
}

void drawTexture (Texture tex, float x, float y, float sx, float sy)
{
	glUseProgram (textureShader.id);
	
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, tex.id);
	
	glm::mat4 transform =
		glm::translate (glm::vec3 (x, y, 0.0f)) *
		glm::scale (glm::vec3 (sx, sy, 1.0f));
	
	glUniform1i (textureLoc, 0);
	glUniformMatrix4fv (transformLoc, 1, GL_FALSE, &transform[0][0]);
	
	glBindVertexArray (textureVao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0x0);
	glBindVertexArray (0);
}

void drawTexture (Texture texture, float2 pos, float2 size, bool flip)
{
	// screen/pixel space to ndc (-1 <-> 1)
	
	float2 frame = getFrame();
	
	float x, y, sx, sy;
	
	x = -0.5 + (-pos.x / frame.x);
	y = -0.5 + ( pos.y / frame.y);
	
	if (size.x == 0 || size.y == 0)
	{
		size = {texture.width, texture.height};
	}
	
	sx = 0.5 * size.x / frame.x;
	sy = 0.5 * size.y / frame.y;
	
	if (flip) sy = -sy;
	
	drawTexture (texture, x, y, sx, sy);
}

void drawTextureFullscreen	(Texture tex, bool flipY, bool flipX)
{
	glUseProgram (textureShader.id);
	
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, tex.id);
	
	glm::mat4 transform =
		glm::translate (glm::vec3 (0, 0, 0)) *
		glm::scale (glm::vec3 ((flipX)? -1:1, (flipY)? -1:1, 1));
	
	transform = glm::mat4 (1);
	
	glUniform1i (textureLoc, 0);
	glUniformMatrix4fv (transformLoc, 1, GL_FALSE, &transform[0][0]);
	
	glBindVertexArray (textureVao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0x0);
	glBindVertexArray (0);
}