#pragma once

#include "define.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Texture
{
	int id;
}

struct Material
{
	Texture texture;
	Texture bump;
	Texture specular;
	
	vec4 diffuse;
	float shiny;
	float roughness;
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

void loadAiMat (aiMaterial& mat)
{
	mat.Get (AI_MATKEY
}

Mesh importMesh (const char * filePath)
{
	using namespace Assimp;
	
	Importer importer;
	const aiScene * scene;
	
	scene = importer.ReadFile (filePath, 
		aiProcess_Triangulate || aiProcess_FlipUVs ||
		aiProcess_GenNormals);
		
	aiNode *	root = scene->mRootNode;
	
	auto mat = scene->mMaterials[0];
}