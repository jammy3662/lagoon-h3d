#include "texture.h"
#include "disk.h"

#include <stdio.h>
#include <glad.h>

#include <stb_image.h>

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