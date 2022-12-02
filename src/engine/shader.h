#pragma once

#include <raylib.h>

int textureSlot = 0;
Shader activeShader;

inline
void shbind(Shader shader)
{
	activeShader = shader;
}

inline
int shloc(char* name)
{
	return GetShaderLocation(activeShader, name);
}

inline
void shvalue(int loc, void* value, int uniformType)
{
	SetShaderValue(activeShader, loc, value, uniformType);
}

inline
void shmatrix(int loc, Matrix matrix)
{
	SetShaderValueMatrix(activeShader, loc, matrix);
}

inline
void shvec(int loc, void* values, int uniformType, int count)
{
	SetShaderValueV(activeShader, loc, values, uniformType, count);
}

inline
void shtexture(int loc, Texture texture, int* slot, int type)
{
	int idx = textureSlot + 12;
	textureSlot += 1;

	rlEnableShader(activeShader.id);
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

void shset(Model* model)
{
	for (int i = 0; i < model->materialCount; i++)
	{
		model->materials[i].shader = activeShader;
	}
}

void shset(Model* model, Shader shader)
{
	for (int i = 0; i < model->materialCount; i++)
	{
		model->materials[i].shader = shader;
	}
}