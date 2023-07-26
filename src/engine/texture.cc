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

"uniform sampler2D tex;"

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
	-.5, .5, 0, 0, 0,		.5, .5,  0, 1, 0,
	-.5, -.5, 0, 0, 1,		.5, -.5, 0, 1, 1,
};
enum {TL, TR, BL, BR};
const uint textureQuadIndices [] =
{
	TR, BR, TL,
	BR, BL, TL,
};

Shader textureShader;
uint textureVao, textureUniformLoc;

void initTextures ()
{
	textureShader = loadShaderCode (textureShaderV, textureShaderF);
	textureUniformLoc = glGetUniformLocation (textureShader.id, "tex");
	
	if (textureShader.compiled)
		printf ("Compiled texture shader\n");
		
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
	if (channels > 5)
	{
		fprintf (stderr, "too many channels %u\n", channels);
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
	
	if (texels)
	{
		printf ("[.] Texture '%s' uploaded\n", path);
		return uploadTexture (w, h, channels, texels);
	}
	
	fprintf (stderr, "[x] Texture '%s' not found\n", path);
	
	return ret;
}

void drawTexture (Texture texture, int x, int y, int w, int h)
{
	glUseProgram (textureShader.id);
	
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, texture.id);
	
	glUniform1i (textureUniformLoc, 0);
	
	glBindVertexArray (textureVao);
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0x0);
	glBindVertexArray (0);
}