#pragma once

#include "define.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>
#include <vector>
#include <string_view>

struct Texture
{
	unsigned int id;
	
	Texture ()
	{
		id = 0;
	}
};

Texture zUploadTex (int w, int h, char* texels)
{
	Texture ret;
	
	glGenTextures (1, &ret.id);
	glBindTexture (GL_TEXTURE_2D, ret.id);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	
	// nearest neighbor for shrinking, interpolate mipmap level
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	// linear for expanding, interpolate mipmap level
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
Texture zLoadAiTex (aiTexture& tex, const aiScene* scene)
{
	// remember loaded textures by load path
	// avoid duplicate textures
	
	const char* file = tex.mFilename.C_Str();
	//printf ("[?] '%s'\n", file);
	
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
			printf ("loaded new texture '%s'\n", file);
			//printf ("\t'%s'\n", data->mFilename.C_Str ());
			// pull texel data from embedded texture
			ret = zUploadTex (data->mWidth, data->mHeight, (char*)data->pcData);
			//printf("inner %i\n", ret.id);
		}
		else
		{
			// texture is not embedded, search for a file on disk
			// TODO:
			
			/*TEMP*/ret.id = (unsigned int) -1;
			fprintf(stderr, "can't find texture '%s'\n", file);
		}
		
		return ret;
	}
}

struct Material
{
	Texture texture;
	Texture normals;
	// Texture specular; // (curr. unsupported)
	
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
		printf("new! '%s'\n", mat.GetName().C_Str ());
		
		Material& ret = zMaterials [std::string (mat.GetName().C_Str ())];
		
		aiColor3D diffuse;
		float opacity;
		
		mat.Get (AI_MATKEY_COLOR_DIFFUSE, diffuse);
		mat.Get (AI_MATKEY_OPACITY, opacity);
		
		ret.diffuse = vec4 {diffuse.r, diffuse.g, diffuse.b, opacity};
		
		mat.Get (AI_MATKEY_SHININESS, ret.shiny);
		mat.Get (AI_MATKEY_REFLECTIVITY, ret.reflection);
		
		if (mat.GetTextureCount (aiTextureType_DIFFUSE) > 0)
		{
			aiTexture texture;
			mat.Get (AI_MATKEY_TEXTURE (aiTextureType_BASE_COLOR, 0), texture);
			
			printf ("'%s' -> '%s'\n", mat.GetName().C_Str (), texture.mFilename.C_Str ());
			
			ret.texture = zLoadAiTex (texture, scene);
			//printf("outer %i\n", ret.texture.id);
		}
		
		if (mat.GetTextureCount (aiTextureType_NORMALS) > 0)
		{
			aiTexture texture;
			mat.Get (AI_MATKEY_TEXTURE (aiTextureType_NORMALS, 0), texture);
			
			printf ("'%s' -> '%s'\n", mat.GetName().C_Str (), texture.mFilename.C_Str ());
			
			ret.normals = zLoadAiTex (texture, scene);
		}
		
		return ret;
	}
}

struct Mesh
{
	unsigned int vao;
	unsigned int vbo, ebo;
	
	struct Vertex
	{
		struct {float x,y,z; } position;
		struct {float x,y,z; } normal;
		struct {unsigned char r,g,b,a; } color;
		struct {float x,y; } uv0;
		struct {float x,y; } uv1;
		struct {float x,y,z; } tangent;
	};
	
	std::vector <Vertex> vertices;
	
	struct Face
	{
		unsigned short a, b, c;
	};
	
	std::vector <Face> faces;
	
	int materialI; // material index
};

Mesh zLoadAiMesh (aiMesh& mesh)
{
	Mesh ret;
	
	ret.vertices.reserve (mesh.mNumVertices);
	
	for (int i = 0; i < ret.vertices.size(); i++)
	{
		auto& position = ret.vertices [i].position;
		position.x = mesh.mVertices [i].x;
		position.y = mesh.mVertices [i].y;
		position.z = mesh.mVertices [i].z;
		
		auto& normal = ret.vertices [i].normal;
		normal.x = mesh.mNormals [i].x;
		normal.x = mesh.mNormals [i].y;
		normal.x = mesh.mNormals [i].z;
		
		auto& color = ret.vertices [i].color;
		color.r = (unsigned char) (255 * mesh.mColors [i]->r);
		color.g = (unsigned char) (255 * mesh.mColors [i]->g);
		color.b = (unsigned char) (255 * mesh.mColors [i]->b);
		color.a = (unsigned char) (255 * mesh.mColors [i]->a);
		
		auto& uv0 = ret.vertices [i].uv0;
		uv0.x = mesh.mTextureCoords [0][i].x;
		uv0.y = mesh.mTextureCoords [0][i].y;
		
		if (mesh.HasTextureCoords (1))
		{
			auto& uv1 = ret.vertices [i].uv1;
			uv1.x = mesh.mTextureCoords [1][i].x;
			uv1.y = mesh.mTextureCoords [1][i].y;
		}
		
		auto& tangent = ret.vertices [i].tangent;
		tangent.x = mesh.mTangents [i].x;
		tangent.y = mesh.mTangents [i].y;
		tangent.z = mesh.mTangents [i].z;
	}
	
	ret.faces.reserve (mesh.mNumFaces);
	
	for (int i = 0; i < ret.faces.size(); i++)
	{
		Mesh::Face& face = ret.faces [i];
		face.a = mesh.mFaces [i].mIndices [0];
		face.b = mesh.mFaces [i].mIndices [1];
		face.c = mesh.mFaces [i].mIndices [2];
	}
	
	glGenVertexArrays (1, &ret.vao);
	glGenBuffers (2, &ret.vbo);
	
	glBindVertexArray (ret.vao);
	
	glBindBuffer (GL_ARRAY_BUFFER, ret.vbo);
	glBufferData (GL_ARRAY_BUFFER, ret.vertices.size() * sizeof (Mesh::Vertex), ret.vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ret.ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, ret.faces.size() * sizeof (Mesh::Face), ret.faces.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (Mesh::Vertex), (void*) offsetof (Mesh::Vertex, position));
	glEnableVertexAttribArray (1);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (Mesh::Vertex), (void*) offsetof (Mesh::Vertex, normal));
	glEnableVertexAttribArray (2);
	glVertexAttribPointer (0, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof (Mesh::Vertex), (void*) offsetof (Mesh::Vertex, position));
	glEnableVertexAttribArray (3);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (Mesh::Vertex), (void*) offsetof (Mesh::Vertex, uv0));
	
	if (mesh.HasTextureCoords (1))
	{
		glEnableVertexAttribArray (4);
		glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (Mesh::Vertex), (void*) offsetof (Mesh::Vertex, uv1));
	}
	
	glEnableVertexAttribArray (5);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (Mesh::Vertex), (void*) offsetof (Mesh::Vertex, tangent));
	
	glBindVertexArray (0);
	
	ret.vertices.clear();
	ret.faces.clear();
	
	return ret;
}

struct Model
{
	std::vector <Material> materials;
	std::vector <Mesh> meshes;
};

Model loadMesh (const char * filePath)
{
	Model ret;
	
	using namespace Assimp;
	Importer importer;
	const aiScene* scene;
	aiNode* root;
	
	scene = importer.ReadFile (filePath, 
		aiProcess_Triangulate ||
		aiProcess_GenNormals || aiProcess_CalcTangentSpace ||
		aiProcess_GenUVCoords || aiProcess_FlipUVs);
		
	root = scene->mRootNode;
	
	ret.materials.reserve (scene->mNumMaterials);
	ret.meshes.reserve (scene->mNumMeshes);
	
	for (int i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* texture = scene->mTextures [i]; 
		zLoadAiTex (*texture, scene);
	}
	
	// grab materials
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial& mat = *scene->mMaterials [i];
		ret.materials [i] = zLoadAiMat (mat, scene);
	}
	
	// TODO: modify this to preserve the node hierarchy present in the original model
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh& data = *scene->mMeshes [i];
		Mesh mesh = zLoadAiMesh (data);
		mesh.materialI = data.mMaterialIndex;
		ret.meshes [i] = mesh;
	}
	
	return ret;
}