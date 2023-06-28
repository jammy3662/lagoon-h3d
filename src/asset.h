#pragma once

#include "define.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Texture
{
	int id;
};

struct Material
{
	Texture texture;
	Texture bump;
	Texture specular;
	
	vec4 diffuse;
	float shiny;
	float reflection;
};

Texture loadAiTex (aiMaterial& mat)
{
	
}

Material loadAiMat (aiMaterial& mat)
{
	Material ret;
	
	aiColor3D diffuse;
	float opacity;
	
	mat.Get (AI_MATKEY_COLOR_DIFFUSE, diffuse);
	mat.Get (AI_MATKEY_OPACITY, opacity);
	
	ret.diffuse = vec4 {diffuse.r, diffuse.g, diffuse.b, opacity};
	
	mat.Get (AI_MATKEY_SHININESS, ret.shiny);
	mat.Get (AI_MATKEY_REFLECTIVITY, ret.reflection);
	
	aiTexture texture;
	mat.Get (AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture);
	
	return ret;
}

struct Mesh
{
	float * positions;
	float * normals;
	char * colors;
	float * uv0;
	float * uv1;
	float * tangents;
	unsigned short * faces;
	
	int vertexCt;
};

Mesh importMesh (const char * filePath)
{
	using namespace Assimp;
	
	Importer importer;
	const aiScene * scene;
	
	scene = importer.ReadFile (filePath, 
		aiProcess_Triangulate || aiProcess_FlipUVs ||
		aiProcess_GenNormals);
		
	aiNode *	root = scene->mRootNode;
	
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial& mat = scene->mMaterials [i];
		
	}
}