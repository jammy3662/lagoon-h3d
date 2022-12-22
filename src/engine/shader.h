#pragma once

#include <raylib.h>

Shader defaultShader;
Shader depthShader;

struct /* SHADER */ {
	
int textureSlot = 0;

inline void init()
{
	defaultShader = LoadShaderFromMemory(0,0);
	depthShader = LoadShaderFromMemory(depthvsShaderCode, depthfsShaderCode);
}

inline void use (Shader shader)
{
	curShader = shader;
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

// int
void attach (char* loc, int* value)
{
	SetShaderValue(curShader, getLoc(loc), value, SHADER_UNIFORM_INT);
}

// float
void attach (char* loc, float* value)
{
	SetShaderValue(curShader, getLoc(loc), value, SHADER_UNIFORM_FLOAT);
}

// vec2
void attach (char* loc, vec2* value)
{
	SetShaderValue(curShader, getLoc(loc), value, SHADER_UNIFORM_VEC2);
}

// vec3
void attach (char* loc, vec3* value)
{
	SetShaderValue(curShader, getLoc(loc), value, SHADER_UNIFORM_VEC3);
}

// vec4
void attach (char* loc, vec4* value)
{
	SetShaderValue(curShader, getLoc(loc), value, SHADER_UNIFORM_VEC4);
}

// matrix
void attach (char* loc, Matrix matrix)
{
	SetShaderValueMatrix(curShader, getLoc(loc), matrix);
}

// array
void attach (char* loc, void* values, int count, int uniformType)
{
	SetShaderValueV(curShader, getLoc(loc), values, uniformType, count);
}

// texture
void attach (char* loc, Texture texture, int type)
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
SHADER;