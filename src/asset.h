#pragma once

#include "define.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>

struct Texture
{
	unsigned int id = (unsigned int)-1;
};

Texture zUploadTex (int w, int h, char* texels)
{
	Texture ret;
	
	glGenTextures (1, &ret.id);
	glBindTexture (GL_TEXTURE_2D, ret.id);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glTexImage2D (GL_TEXTURE_2D, 2, GL_RGBA, w, h,
	/*border*/0, GL_BGRA, GL_UNSIGNED_BYTE, texels);
	glGenerateMipmap (GL_TEXTURE_2D);
	
	return ret;
}

// pool of uploaded textures
std::unordered_map
	<std::string, Texture> zTextures;

// load referenced/embedded textures from aiMaterial(s)
Texture zLoadAiTex (aiTexture tex, const aiScene* scene)
{
	// remember loaded textures by load path
	// avoid duplicate textures
	
	const char* file = tex.mFilename.C_Str();
	
	// grab previously loaded texture
	try
	{
		return zTextures.at (std::string (file));
	}
	
	// otherwise, new texture from image data
	catch (std::out_of_range _)
	{
		// map allocates a new texture
		Texture& ret = zTextures [std::string (file)];
		
		const aiTexture* data = scene->GetEmbeddedTexture (file);
		
		if (data != 0)
		{
			// pull texel data from embedded texture
			ret = zUploadTex (data->mWidth, data->mHeight, (char*)data->pcData);
		}
		else
		{
			// texture is not embedded, search for a file on disk
			// TODO:
			
			/*TEMP*/ret.id = (unsigned int) -1;
		}
		
		return ret;
	}
}

struct Material
{
	Texture texture;
	Texture normals;
	Texture specular; // (curr. unsupported)
	
	vec4 diffuse;
	float shiny;
	float reflection;
};

// loaded materials
std::unordered_map
	<std::string, Material> zMaterials;

Material zLoadAiMat (aiMaterial& mat, const aiScene* scene)
{
	// retrieve stored material
	try
	{
		return zMaterials.at (std::string (mat.GetName().C_Str ()));
	}
	
	// populate new material with importer data
	catch (std::out_of_range _)
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
		mat.Get (AI_MATKEY_TEXTURE (aiTextureType_DIFFUSE, 0), texture);
		ret.texture = zLoadAiTex (texture, scene);
		
		// re-use the aiTexture struct from before to save space and time
		mat.Get (AI_MATKEY_TEXTURE (aiTextureType_NORMALS, 0), texture);
		ret.normals = zLoadAiTex (texture, scene);
		
		return ret;
	}
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
		aiMaterial& mat = *scene->mMaterials [i];
		zLoadAiMat (mat, scene);
	}
}