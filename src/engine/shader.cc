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

void useShader (Shader s)
{
	shader = s;
	glUseProgram (s.id);
}

uint uniformLocation (const char* name)
{
	try
	{
		return shader.uniforms.at (std::string (name));
	}
	
	catch (std::out_of_range err)
	{
		// first access to this uniform
		// add to uniform list
		
		uint ret =
		shader.uniforms [std::string (name)] = glGetUniformLocation (shader.id, name);
		
		if (ret == -1) fprintf (stderr, "[x] Uniform '%s' not found\n", name);
		return ret;
	}
}

void setUniform (const char* name, int i)
{
	glUniform1i (uniformLocation (name), i);
}

void setUniform (const char* name, float f)
{
	glUniform1f (uniformLocation (name), f);
}

void setUniform (const char* name, double d)
{
	glUniform1f (uniformLocation (name), (float) d);
}

void setUniform (const char* name, float x, float y)
{
	glUniform2f (uniformLocation (name), x, y);
}

void setUniform (const char* name, float x, float y, float z)
{
	glUniform3f (uniformLocation (name), x, y, z);
}

void setUniform (const char* name, float x, float y, float z, float w)
{
	glUniform4f (uniformLocation (name), x, y, z, w);
}

// TODO:	(incomplete)
void setUniform (const char* name, Texture t)
{
	glBindTexture (GL_TEXTURE_2D, t.id);
	glUniform1i (uniformLocation (name), t.id);
}