#pragma once

#include "backend.h"

struct Shader
{
	int id;
};

// load shader code from memory
// (provide raw, null-terminated char pointers)
Shader loadShader (char* vertexCode, char* fragmentCode)
{
	Shader s;
	
	int vertex, fragment;
	
	// vertex //
	{
		char errorLog [1024] = {0};
		
		vertex = glCreateShader (GL_VERTEX_SHADER);
		glShaderSource (vertex, 1, &vertexCode, 0);
		glCompileShader (vertex);
		glGetShaderInfoLog (vertex, sizeof (errorLog), 0, errorLog);
		if (errorLog [0]) fprintf (stderr, "%s\n", errorLog);
	}
	
	// fragment //
	{
		char errorLog [1024] = {0};
		
		fragment = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (fragment, 1, &fragmentCode, 0);
		glCompileShader (fragment);
		glGetShaderInfoLog (fragment, sizeof (errorLog), 0, errorLog);
		if (errorLog [0]) fprintf (stderr, "%s\n", errorLog);
	}
	
	// final linking //
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

// load GLSL shader files
Shader loadShaderSource (char* vertexPath, char* fragmentPath)
{
	Shader s;
	
	// vertex //
	FILE* vFile = fopen (vertexPath, "r");
	if (! vFile)
		fprintf (stderr, "Can't open vertex shader '%s'\n", vertexPath);
	
	long vLen;
	char* vBuffer;
	
	fseek (vFile , 0, SEEK_END);
	vLen = ftell (vFile);
	rewind (vFile);

	vBuffer = (char*) calloc (vLen +	1, sizeof (char));
	fread (vBuffer, vLen, sizeof (char), vFile);
	
	// fragment //
	FILE * fFile = fopen (fragmentPath, "r");
	if (! fFile)
		fprintf (stderr, "Can't open fragment shader '%s'\n", fragmentPath);
	
	long fLen;
	char* fBuffer;
	
	fseek (fFile , 0, SEEK_END);
	fLen = ftell (fFile);
	rewind (fFile);

	fBuffer = (char*) calloc (fLen +	1, sizeof (char));
	fread (fBuffer, fLen, sizeof (char), fFile);

	fprintf (stdout, "[…] Compiling '%s' and '%s'...\n", vertexPath, fragmentPath);
	s = loadShader (vBuffer, fBuffer);
	fprintf (stdout, "[✔] Compiled '%s' and '%s'\n", vertexPath, fragmentPath);

	// file / buffer cleanup
	fclose (vFile);
	free (vBuffer);
	fclose (fFile);
	free (fBuffer);
	
	return s;
}

inline
void setShaderInt (int i, char* uniform, Shader shader)
{
	glUniform1i (glGetUniformLocation (shader.id, uniform), i);
}

inline
void setShaderFloat (float f, char* uniform, Shader shader)
{
	glUniform1f (glGetUniformLocation (shader.id, uniform), f);
}

inline
void setShaderVec2 (float r, float g, char* uniform, Shader shader)
{
	glUniform2f (glGetUniformLocation (shader.id, uniform), r, g);
}

inline
void setShaderVec3 (float r, float g, float b, char* uniform, Shader shader)
{
	glUniform3f (glGetUniformLocation (shader.id, uniform), r, g, b);
}

inline
void setShaderVec4 (float r, float g, float b, float a, char* uniform, Shader shader)
{
	glUniform4f (glGetUniformLocation (shader.id, uniform), r, g, b, a);
}