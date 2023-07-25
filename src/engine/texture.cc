#include "texture.h"
#include "shader.h"
#include "disk.h"

#include <stdio.h>
#include <glad.h>

#include <stb_image.h>

const char* textureShaderV =
"#version 330 core\n"

"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec2 uv;"

"out vec2 UV;"

"void main ()"
"{"
"	gl_Position = vec4 (pos.x, pos.y, pos.z, 1.0);"
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
	-0.5, 0.5, 0, 1, // top left
	0.5, 0.5,  1, 1, // top right
	0.5, -0.5, 0, 0, // bottom left
	0.5, -0.5, 1, 0, // bottom right
};

const uint textureQuadIndices [] =
{
	1, 3, 0,
	3, 2, 0,
};

Shader textureShader;
uint textureVao;

void initTextures ()
{
	textureShader = loadShaderCode (textureShaderV, textureShaderF);
	
	uint vao, vbo, ebo;
	
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	
	glGenBuffers (2, &vbo);
	glBindBuffer (GL_VERTEX_ARRAY, vbo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	// position
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void*)0);
	// uv
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (float), (float*)2);
	
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), textureQuadIndices, GL_STATIC_DRAW);
	
	// unbind for safety
	glBindVertexArray (0);
	
	textureVao = vao;
}

Texture uploadTexture (int w, int h, int channels, char* texels)
{
	Texture ret;

	glGenTextures (1, &ret.id);
	glBindTexture (GL_TEXTURE_2D, ret.id);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// linear for shrinking, interpolate mipmap level
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	// nearest for expanding, interpolate mipmap level
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// TODO:	replace with error handling/messsage
	if (channels > 5) channels = 1;
	
	// 0 element for padding (1 channel = GL_R)
	int fmt [] = {GL_R, GL_R, GL_RG, GL_RGB, GL_RGBA};
	
	glTexImage2D (GL_TEXTURE_2D, 2, GL_RGBA, w, h,
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
	
	if (texels)
		return uploadTexture (w, h, channels, texels);
	
	fprintf (stderr, "[x] Texture '%s' not found\n", path);
	
	return ret;
}

void drawTexture (Texture texture, int x, int y, int w, int h)
{
	useShader (textureShader);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, texture.id);
	
	glBindVertexArray (textureVao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0x0);
	glBindVertexArray (0);
}