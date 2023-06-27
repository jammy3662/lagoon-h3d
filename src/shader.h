#pragma once

#include "backend.h"

#include <stdexcept>
#include <unordered_map>

struct Shader 
{
	int id;
	
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
		<char*, Uniform> uniforms;

	Uniform operator [] (char* name)
	{
		try
		{
			return uniforms.at (name);
		}
		catch (std::out_of_range _)
		{
			// the map will auto add the new element
			Uniform u = uniforms [name];
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
// (provide raw, null-terminated char pointers)
Shader loadShader (char* vertexCode, char* fragmentCode)
{
	int vertex;
	{
		char errorLog [1024] = {0};
		
		vertex = glCreateShader (GL_VERTEX_SHADER);
		glShaderSource (vertex, 1, &vertexCode, 0);
		glCompileShader (vertex);
		glGetShaderInfoLog (vertex, sizeof (errorLog), 0, errorLog);
		if (errorLog [0]) fprintf (stderr, "%s\n", errorLog);
	}
	
	int fragment;
	{
		char errorLog [1024] = {0};
		
		fragment = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (fragment, 1, &fragmentCode, 0);
		glCompileShader (fragment);
		glGetShaderInfoLog (fragment, sizeof (errorLog), 0, errorLog);
		if (errorLog [0]) fprintf (stderr, "%s\n", errorLog);
	}
	
	Shader s;
	{
		char errorLog [1024] = {0};
		
		s.id = glCreateProgram ();
		glAttachShader (s.id, vertex);
		glAttachShader (s.id, fragment);
		glLinkProgram (s.id);
		
		glGetProgramInfoLog (s.id, sizeof (errorLog), 0, errorLog);
		if (errorLog [0]) fprintf (stderr, "%s\n", errorLog);
	}
	
	return s;
}

char* fileToBuf (char* path)
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
	
	char* vertex = fileToBuf (vertexPath);
	if (!vertex) fprintf (stderr, "Can't open vertex shader '%s'\n", vertexPath);
	
	char* fragment = fileToBuf (fragmentPath);
	if (!fragment) fprintf (stderr, "Can't open fragment shader '%s'\n", fragmentPath);

	s = loadShader (vertex, fragment);

	fprintf (stdout, "[•] Compiled '%s' and '%s'\n", vertexPath, fragmentPath);

	free (vertex);
	free (fragment);
	
	return s;
}