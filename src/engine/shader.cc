#include "shader.h"

#include <stdexcept>
#include <glad.h>

// helper function to load text from file
// TODO:	use resource manager to load
char* fileToBuffer (const char* path)
{
	FILE* file = fopen (path, "r");
	if (!file) return (char*) 0;
	
	char* buffer;
	long len;
	
	fseek (file , 0, SEEK_END);
	len = ftell (file);
	rewind (file);

	// use calloc, not malloc, to ensure a 0 byte at the end
	buffer = (char*) calloc (len + 1, sizeof (char));
	fread (buffer, len, sizeof (char), file);
	
	fclose (file);
	
	return buffer;
}

// uniform type
enum UT
{
	i,
	f,
	d,
	xy,
	xyz,
	xyzw,
	t,
};

// curent shader
Shader shader;
int textureUnit = 0;

// load shader code from memory
// (raw, null-terminated char pointers)
Shader uploadShader (const char* vsCode, const char* fsCode)
{
	Shader ret;
	ret.compiled = 1;
	
	int vertex;
	{
		char errorLog [1024] = {0};
		
		vertex = glCreateShader (GL_VERTEX_SHADER);
		glShaderSource (vertex, 1, &vsCode, 0);
		glCompileShader (vertex);
		glGetShaderInfoLog (vertex, sizeof (errorLog), 0, errorLog);
		if (errorLog [0])
		{
			fprintf (stderr, "%s\n", errorLog);
			ret.compiled = 0;
		}
	}
	
	int fragment;
	{
		char errorLog [1024] = {0};
		
		fragment = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (fragment, 1, &fsCode, 0);
		glCompileShader (fragment);
		glGetShaderInfoLog (fragment, sizeof (errorLog), 0, errorLog);
		if (errorLog [0])
		{
			fprintf (stderr, "%s\n", errorLog);
			ret.compiled = 0;
		}
	}
	
	// link code into shader program
	{
		char errorLog [1024] = {0};
		
		ret.id = glCreateProgram ();
		glAttachShader (ret.id, vertex);
		glAttachShader (ret.id, fragment);
		glLinkProgram (ret.id);
		
		glGetProgramInfoLog (ret.id, sizeof (errorLog), 0, errorLog);
		if (errorLog [0])
		{
			fprintf (stderr, "%s\n", errorLog);
			ret.compiled = 0;
		}
	}
	
	return ret;
}

Shader loadShader (const char* vsPath, const char* fsPath)
{
	
	Shader ret;
	
	char* vertex = fileToBuffer (vsPath);
	if (!vertex) fprintf (stderr, "Can't open vertex shader '%s'\n", vsPath);
	
	char* fragment = fileToBuffer (fsPath);
	if (!fragment) fprintf (stderr, "Can't open fragment shader '%s'\n", fsPath);

	ret = uploadShader (vertex, fragment);
	
	free (vertex);
	free (fragment);
	
	if (ret.compiled)
	{
		fprintf (stdout, "[•] Compiled '%s' and '%s'\n", vsPath, fsPath);
	}
	
	else
	{
		fprintf (stderr, "[x] Shader compile error in '%s' and/or '%s'\n", vsPath, fsPath);
	}
	
	return ret;
}

void bindShader (Shader s)
{
	shader = s;
	glUseProgram (s.id);
	
	// TODO: bind shader uniforms as stored in shader struct
}

void attach (Shader shader, const char* name, int i)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::i;
	u.value = 0x0;
	u.constant.i = i;
}

void attach (Shader shader, const char* name, float f)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::f;
	u.value = 0x0;
	u.constant.f = f;
}

void attach (Shader shader, const char* name, double d)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::d;
	u.value = 0x0;
	u.constant.d = d;
}

void attach (Shader shader, const char* name, float2 xy)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::xy;
	u.value = 0x0;
	u.constant.xy = xy;
}

void attach (Shader shader, const char* name, float3 xyz)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::xyz;
	u.value = 0x0;
	u.constant.xyz = xyz;
}

void attach (Shader shader, const char* name, float4 xyzw)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::xyzw;
	u.value = 0x0;
	u.constant.xyzw = xyzw;
}

void attach (Shader shader, const char* name, int* i)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::i;
	u.value = i;
}

void attach (Shader shader, const char* name, float* f)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::f;
	u.value = f;
}

void attach (Shader shader, const char* name, double* d)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::d;
	u.value = d;
}

void attach (Shader shader, const char* name, float2* xy)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::xy;
	u.value = xy;
}

void attach (Shader shader, const char* name, float3* xyz)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::xyz;
	u.value = xyz;
}

void attach (Shader shader, const char* name, float4* xyzw)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::xyzw;
	u.value = xyzw;
}

void attach (Shader shader, const char* name, Texture t)
{
	Uniform u;
	u.location = glGetUniformLocation (shader.id, name);
	u.type = UT::t;
	u.value = &t.id;
}