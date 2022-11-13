#pragma once

#include <GL/glew.h>
#include <iostream>

class Mesh
{
public:
	Mesh();

	Mesh(unsigned int* indices, unsigned int numOfIndices, float* vertices, unsigned int numOfVertices);

	void CreateMesh(unsigned int* indices, unsigned int m_NumOfIndices, float* vertices, unsigned int m_NumOfVertices);
	void Render();

	void ClearMesh();

	~Mesh();

private:
	GLuint m_VAO, m_VBO, m_IBO;

	float* m_Vertices;
	unsigned int * m_Indices;
	unsigned int m_NumOfIndices, m_NumOfVertices;
};

