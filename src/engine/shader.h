#pragma once

#include <string>
#include <unordered_map>

#include "texture.h"

struct Shader 
{
	uint id;
	char compiled;
	
	std::unordered_map
		<std::string, uint> uniforms;
};

// current shader
extern Shader shader;

// load vertex/fragment GLSL shader files from paths
Shader loadShader (const char* vsPath, const char* fsPath);

void useShader (Shader s);

void setUniform (const char* name, int i);
void setUniform (const char* name, float f);
void setUniform (const char* name, double d);
void setUniform (const char* name, float x, float y);
void setUniform (const char* name, float x, float y, float z);
void setUniform (const char* name, float x, float y, float z, float w);
void setUniform (const char* name, Texture t);