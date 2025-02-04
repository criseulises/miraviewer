//-----------------------------------------------------------------------------
// Mesh.h by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Basic Mesh class
//-----------------------------------------------------------------------------
#pragma once

#include <vector>
#include <string>
#ifdef __APPLE__
#include <glad/glad.h>
#else
#define GLEW_STATIC
#include "GL/glew.h"	
#endif
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

struct SubMesh
{
	std::vector<Vertex> vertices;
	GLuint VBO, VAO;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void loadModel(const std::string& filename);
	void draw();

private:
	void initBuffers(SubMesh& mesh);

	bool mLoaded;
	std::vector<SubMesh> mMeshes;
};