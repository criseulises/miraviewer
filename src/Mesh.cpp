//-----------------------------------------------------------------------------
// Mesh.cpp by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Basic Mesh class
//-----------------------------------------------------------------------------
#include "Mesh.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh() : mLoaded(false) {}

Mesh::~Mesh()
{
	for (auto& mesh : mMeshes)
	{
		glDeleteVertexArrays(1, &mesh.VAO);
		glDeleteBuffers(1, &mesh.VBO);
	}
}

void Mesh::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;
    mMeshes.clear();  // Limpiar antes de cargar nuevas mallas

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) 
    {
        aiMesh* mesh = scene->mMeshes[i];
        SubMesh subMesh;

        for (unsigned int j = 0; j < mesh->mNumVertices; j++) 
        {
            Vertex aVertex;
            aVertex.position = glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);

            if (mesh->HasTextureCoords(0))
            {
                aVertex.texCoords = glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
            }

            subMesh.vertices.push_back(aVertex);
        }

        initBuffers(subMesh);
        mMeshes.push_back(subMesh);
    }

    std::cout << "Model has been loaded with " << mMeshes.size() << " meshes." << std::endl;
    mLoaded = true;
}

void Mesh::initBuffers(SubMesh& mesh)
{
	glGenVertexArrays(1, &mesh.VAO);
	glGenBuffers(1, &mesh.VBO);

	glBindVertexArray(mesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void Mesh::draw()
{
	if (!mLoaded) return;

	for (const auto& mesh : mMeshes)
	{
		glBindVertexArray(mesh.VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
		glBindVertexArray(0);
	}
}