#pragma once

#include <vector>

#include "texture.h"

struct Uniform
{
	uint location; // shader uniform index
	uint type; // int, float, texture, etc
	
	union Value
	{
		int i; float f; double d;
		float2 xy; float3 xyz;
		float4 xyzw; Texture t;
	};
	
	Value constant;
	
	void* value;
};

struct Shader 
{
	uint id;
	char compiled;
	
	std::vector <Uniform> uniforms;
};

// current shader
extern Shader shader;

// load vertex/fragment GLSL shader files from paths
Shader loadShader (const char* vsPath, const char* fsPath);

void bindShader (Shader s);

void attach (Shader shader, const char* uniform_name, int i);
void attach (Shader shader, const char* uniform_name, float f);
void attach (Shader shader, const char* uniform_name, double d);
void attach (Shader shader, const char* uniform_name, float2 xy);
void attach (Shader shader, const char* uniform_name, float3 xyz);
void attach (Shader shader, const char* uniform_name, float4 xyzw);

void attach (Shader shader, const char* uniform_name, int* i);
void attach (Shader shader, const char* uniform_name, float* f);
void attach (Shader shader, const char* uniform_name, double* d);
void attach (Shader shader, const char* uniform_name, float2* xy);
void attach (Shader shader, const char* uniform_name, float3* xyz);
void attach (Shader shader, const char* uniform_name, float4* xyzw);
void attach (Shader shader, const char* uniform_name, Texture t);
