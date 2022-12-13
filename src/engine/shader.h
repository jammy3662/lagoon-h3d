#pragma once

#include <raylib.h>

int textureSlot = 0;

inline
int shloc(char* name)
{
	return GetShaderLocation(curShader, name);
}

inline
void shvalue(int loc, void* value, int uniformType)
{
	SetShaderValue(curShader, loc, value, uniformType);
}

inline
void shmatrix(int loc, Matrix matrix)
{
	SetShaderValueMatrix(curShader, loc, matrix);
}

inline
void shvec(int loc, void* values, int uniformType, int count)
{
	SetShaderValueV(curShader, loc, values, uniformType, count);
}

inline
void shtexture(int loc, Texture texture, int type)
{
	int idx = textureSlot + 12;
	textureSlot += 1;

	rlEnableShader(curShader.id);
	rlActiveTextureSlot(idx);
	#if defined(GRAPHICS_API_OPENGL_11)
		glEnable(type);
	#endif
	glBindTexture(type, texture.id);
	rlSetUniform(loc, &idx, SHADER_UNIFORM_INT, 1);
}

inline
void shresetTextures()
{
	rlDisableTexture();
	textureSlot = 0;
}