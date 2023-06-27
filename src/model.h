#pragma once

#include "define.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Mesh
{
	float * positions;
	float * normals;
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
}