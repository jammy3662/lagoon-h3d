#pragma once

#include <raylib.h>

Shader defaultShader;
Shader depthShader;

struct _SHADER {
	
int textureSlot = 0;

inline void init()
{
	defaultShader = LoadShaderFromMemory(0,0);
	depthShader = LoadShaderFromMemory(depthvsShaderCode, depthfsShaderCode);
}

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

// value (int location)
inline void attach (int loc, void* value, int uniformType)
{
	SetShaderValue(curShader, loc, value, uniformType);
}

// matrix (int location)
inline void attach (int loc, Matrix matrix)
{
	SetShaderValueMatrix(curShader, loc, matrix);
}

// array (int location)
inline void attach (int loc, void* values, int count, int uniformType)
{
	SetShaderValueV(curShader, loc, values, uniformType, count);
}

// texture (int location)
inline void attach (int loc, Texture texture, int type)
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

} 
SHADER;