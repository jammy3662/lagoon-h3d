#pragma once

#include <raylib.h>

struct _shader {
	
int textureSlot = 0;

inline void use (Shader shader)
{
	curShader = shader;
	rlDisableTexture();
	textureSlot = 0;
}

// get uniform location by name
inline int getLoc (char* name)
{
	return GetShaderLocation(curShader, name);
}

// value
inline void attach (char* loc, void* value, int uniformType)
{
	SetShaderValue(curShader, getLoc(loc), value, uniformType);
}

// matrix
inline void attach (char* loc, Matrix matrix)
{
	SetShaderValueMatrix(curShader, getLoc(loc), matrix);
}

// array
inline void attach (char* loc, void* values, int count, int uniformType)
{
	SetShaderValueV(curShader, getLoc(loc), values, uniformType, count);
}

// texture
inline void attach (char* loc, Texture texture, int type)
{
	int idx = textureSlot + 12;
	textureSlot += 1;

	rlEnableShader(curShader.id);
	rlActiveTextureSlot(idx);
	#if defined(GRAPHICS_API_OPENGL_11)
		glEnable(type);
	#endif
	glBindTexture(type, texture.id);
	rlSetUniform(getLoc(loc), &idx, SHADER_UNIFORM_INT, 1);
}

} 
shader;