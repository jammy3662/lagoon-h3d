#pragma once

#include <raylib.h>

Shader defaultShader;
Shader depthShader;

int shaderTextureSlot = 0;

struct ShaderPass : public virtual Shader {

void operator=(Shader _shader)
{
	*(Shader*)this = _shader;
}

Shader handle()
{
	return *(Shader*)this;
}

struct Attachment
{
//	typedef ShaderUniformDataType Type;
	
	int loc;
	int type;
	void* value;
};

enum
{
	SHADER_UNIFORM_MATRIX = SHADER_UNIFORM_SAMPLER2D + 1,
	SHADER_UNIFORM_TEXTURE2D,
	SHADER_UNIFORM_TEXTURE3D,
	SHADER_UNIFORM_TEXTURE_CUBE,
};

Array<Attachment> uniforms;

void _attachValue(char* loc, void* value, int type)
{
	Attachment uniform = {
		.loc = GetShaderLocation(handle(), loc),
		.value = value,
		.type = type
	};
	
	uniforms.append(uniform);
}

// int
inline void attach(char* loc, int* int_)
{
	_attachValue(loc, int_, SHADER_UNIFORM_INT);
}

// float
inline void attach(char* loc, float* float_)
{
	_attachValue(loc, float_, SHADER_UNIFORM_FLOAT);
}

// vec2
void attach (char* loc, vec2* vec2_)
{
	_attachValue(loc, vec2_, SHADER_UNIFORM_VEC2);
}

// vec3
void attach (char* loc, vec3* vec3_)
{
	_attachValue(loc, vec3_, SHADER_UNIFORM_VEC3);
}

// vec4
void attach (char* loc, vec4* vec4_)
{
	_attachValue(loc, vec4_, SHADER_UNIFORM_VEC4);
}

// matrix
void attach (char* loc, Matrix* matrix_)
{
	_attachValue(loc, matrix_, SHADER_UNIFORM_MATRIX);
}

// texture
void attach (char* loc, Texture* texture_, int type)
{
	_attachValue(loc, texture_, type);
}

void _bindTexture(int loc, Texture texture, int type)
{
	int idx = shaderTextureSlot + 12;
	shaderTextureSlot++;
	
	rlEnableShader(handle().id);
	rlActiveTextureSlot(idx);
	#if defined(GRAPHICS_API_OPENGL_11)
		glEnable(type);
	#endif
	
	glBindTexture(type, texture.id);
	rlSetUniform(loc, &idx, SHADER_UNIFORM_INT, 1);
}

void bind()
{
	// dont bind the same shader twice
	if (handle().id == curShader.id) return;
	
	curShader = handle();
	rlSetShader(handle().id, handle().locs);
	shaderTextureSlot = 0;
}

void attach()
{
	for (int i = 0; i < uniforms.size; i++)
	{
	Attachment u = uniforms[i];
	
	switch (u.type)
	{
	case SHADER_UNIFORM_MATRIX:
		SetShaderValueMatrix(handle(), u.loc, *(Matrix*) u.value);
		break;
	case SHADER_UNIFORM_TEXTURE2D:
		_bindTexture(u.loc, *(Texture*) u.value, GL_TEXTURE_2D);
		break;
	case SHADER_UNIFORM_TEXTURE3D:
		_bindTexture(u.loc, *(Texture*) u.value, GL_TEXTURE_3D);
		break;
	case SHADER_UNIFORM_TEXTURE_CUBE:
		_bindTexture(u.loc, *(Texture*) u.value, GL_TEXTURE_CUBE_MAP);
		break;
	default:
		SetShaderValue(handle(), u.loc, u.value, u.type);
		break;
	}
	}
}

} /* ShaderPass */ ;

void shinit()
{
	defaultShader = LoadShader(0,0);
	depthShader = LoadShaderFromMemory(depthvsShaderCode, depthfsShaderCode);
}

void shuse(Shader shader)
{
	curShader = shader;
	rlSetShader(shader.id, shader.locs);
	shaderTextureSlot = 0;
}

inline
int shloc(Shader shader, char* name)
{
	return GetShaderLocation(shader, name);
}

void shset(Shader shader, char* name, int int_)
{
	int value = int_;
	SetShaderValue(shader, shloc(shader, name), &value, SHADER_UNIFORM_INT);
}

void shset(Shader shader, char* name, float float_)
{
	float value = float_;
	SetShaderValue(shader, shloc(shader, name), &value, SHADER_UNIFORM_FLOAT);
}

void shset(Shader shader, char* name, vec2 vec2_)
{
	vec2 value = vec2_;
	SetShaderValue(shader, shloc(shader, name), &value, SHADER_UNIFORM_VEC2);
}
void shset(Shader shader, char* name, vec3 vec3_)
{
	vec3 value = vec3_;
	SetShaderValue(shader, shloc(shader, name), &value, SHADER_UNIFORM_VEC3);
}
void shset(Shader shader, char* name, vec4 vec4_)
{
	vec4 value = vec4_;
	SetShaderValue(shader, shloc(shader, name), &value, SHADER_UNIFORM_VEC4);
}

void shset(Shader shader, char* name, Matrix matrix_)
{
	SetShaderValueMatrix(shader, shloc(shader, name), matrix_);
}

void shset(Shader shader, char* name, Texture texture, int type)
{
	int idx = shaderTextureSlot + 12;
	shaderTextureSlot++;
	
	rlEnableShader(shader.id);
	rlActiveTextureSlot(idx);
	#if defined(GRAPHICS_API_OPENGL_11)
		glEnable(type);
	#endif
	
	glBindTexture(type, texture.id);
	rlSetUniform(shloc(shader, name), &idx, SHADER_UNIFORM_INT, 1);
}