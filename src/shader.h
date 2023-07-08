#pragma once

#include "backend.h"

#include <stdexcept>
#include <unordered_map>

struct Shader 
{
	int id;
	char compiled;
	
	struct Uniform
	{
		int loc;
		
		void operator = (int i)
		{
			glUniform1i (loc, i);
		}
		
		void operator = (float f)
		{
			glUniform1f (loc, f);
		}
		
		void operator = (double d)
		{
			glUniform1f (loc, (float) d);
		}
		
		void operator = (vec2 v2)
		{
			glUniform2f (loc, v2.x, v2.y);
		}
		
		void operator = (vec3 v3)
		{
			glUniform3f (loc, v3.x, v3.y, v3.z);
		}
		
		void operator = (vec4 v4)
		{
			glUniform4f (loc, v4.x, v4.y, v4.z, v4.w);
		}
	};
	
	std::unordered_map
		<std::string, Uniform> zUniforms = {{}};

	Uniform operator [] (char* name)
	{
		try
		{
			return zUniforms.at (std::string (name));
		}
		catch (std::out_of_range _)
		{
			// the map will auto-add the new element
			Uniform& u = zUniforms [std::string (name)];
			u.loc = glGetUniformLocation (id, name);
			if (u.loc == -1) fprintf (stderr, "[x] Uniform '%s' not found\n", name);
			return u;
		}
	}
};

inline
void shader (Shader s)
{
	glUseProgram (s.id);
}

// load shader code from memory
// (raw, null-terminated char pointers)
Shader zLoadShader (char* vertexCode, char* fragmentCode)
{
	Shader ret;
	
	int vertex;
	{
		char errorLog [1024] = {0};
		
		vertex = glCreateShader (GL_VERTEX_SHADER);
		glShaderSource (vertex, 1, &vertexCode, 0);
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
		glShaderSource (fragment, 1, &fragmentCode, 0);
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

char* zFileToBuf (char* path)
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

// load GLSL shader files
Shader loadShaderSource (char* vertexPath, char* fragmentPath)
{
	Shader s;
	
	char* vertex = zFileToBuf (vertexPath);
	if (!vertex) fprintf (stderr, "Can't open vertex shader '%s'\n", vertexPath);
	
	char* fragment = zFileToBuf (fragmentPath);
	if (!fragment) fprintf (stderr, "Can't open fragment shader '%s'\n", fragmentPath);

	s = zLoadShader (vertex, fragment);
	
	if (s.compiled)
	{
		fprintf (stdout, "[•] Compiled '%s' and '%s'\n", vertexPath, fragmentPath);
	}
	else
	{
		fprintf (stderr, "[x] Shader compile error in '%s' and/or '%s'\n", vertexPath, fragmentPath);
	}
	
	free (vertex);
	free (fragment);

	

	return s;
}