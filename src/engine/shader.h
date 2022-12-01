#pragma once

#include <raylib.h>

int textureSlot = 0;

struct shader: public virtual Shader
{
	int loc(char* name);

	void setValue(int loc, void* value, int uniformType);

	void setMatrix(int loc, Matrix matrix);

	void setVector(int loc, void* values, int uniformType, int count);

	void setTexture(int loc, Texture texture, int* slot, int type);
};

int shader::loc(char* name)
{
	return GetShaderLocation(*this, name);
}

void shader::setValue(int loc, void* value, int uniformType)
{
	SetShaderValue(*this, loc, value, uniformType);
}

void shader::setMatrix(int loc, Matrix matrix)
{
	SetShaderValueMatrix(*this, loc, matrix);
}

void shader::setVector(int loc, void* values, int uniformType, int count)
{
	SetShaderValueV(*this, loc, values, uniformType, count);
}

void shader::setTexture(int loc, Texture texture, int* slot, int type)
{
	int idx = textureSlot + 12;
	textureSlot += 1;

	rlEnableShader(this->id);
	rlActiveTextureSlot(idx);
	#if defined(GRAPHICS_API_OPENGL_11)
		glEnable(type);
	#endif
	glBindTexture(type, texture.id);
	rlSetUniform(loc, &idx, SHADER_UNIFORM_INT, 1);
}

inline
void resetTextures()
{
	rlDisableTexture();
	textureSlot = 0;
}