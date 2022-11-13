#include "Mesh.h"

Mesh::Mesh()
{
	m_VAO = 0;
	m_VBO = 0;
	m_IBO = 0;

	m_NumOfIndices = 0;
	m_NumOfVertices = 0;
}

Mesh::Mesh(unsigned int* indices, unsigned int numOfIndices, float* vertices, unsigned int numOfVertices)
{
	m_VAO = 0;
	m_VBO = 0;
	m_IBO = 0;

	CreateMesh(indices, numOfIndices, vertices, numOfVertices);
}

void Mesh::CreateMesh(unsigned int* indices, unsigned int numOfIndices, float* vertices, unsigned int numOfVertices)
{
	m_Indices = indices; 
	m_NumOfIndices = numOfIndices;
	m_Vertices = vertices;
	m_NumOfVertices = numOfVertices;

	//std::cout << m_NumOfIndices << std::endl;
	//std::cout << m_NumOfVertices << std::endl;

	// Generate and bind vertex array object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Generate, bind and set index buffer
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumOfIndices, m_Indices, GL_STATIC_DRAW);

	// Generate and bind vertex buffer object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_NumOfVertices, m_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(m_Vertices[0]) * 4, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(m_Vertices[0]) * 4, (void*)(sizeof(vertices[0]) * 2));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::Render()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glDrawElements(GL_TRIANGLES, m_NumOfIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (m_IBO != 0)
	{
		glDeleteBuffers(1, &m_IBO);
		m_IBO = 0;
	}

	if (m_VBO != 0)
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}

	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	m_NumOfIndices = 0;
}


Mesh::~Mesh()
{
	ClearMesh();
}