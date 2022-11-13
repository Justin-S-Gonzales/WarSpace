#pragma once

#include "Object.h"
#include "GLFW/glfw3.h"

#include <vector>
#include <list>
#include <iostream>

class TetrisPiece : public Object
{
private:
	unsigned int m_CurrentRotation;
	std::vector<std::list<Object>> m_Rotations;
	unsigned int m_GridSize;
	unsigned int m_NumberOfRotations;
	bool m_IsFalling;
	glm::vec2 m_GridStart;

	GLfloat m_TimeAcc1 = 0.0f;
	GLfloat m_TimeAcc2 = 0.0f;
	GLfloat m_TimeAcc3 = 0.0f;

public:
	enum class Tetromino { I, O, T, J, L, S, Z };

	TetrisPiece(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, glm::vec4 color,
		Tetromino tetromino, unsigned int gridSize);

	void Start();

	void Update(bool* keys, GLfloat& deltaTime, GLfloat& moveSpeed, GLfloat& fallSpeed, GLfloat& rotationSpeed,
		std::shared_ptr<Object> grid, std::list <Object*> fallenTiles, GLfloat initFallSpeed);

	std::list<Object> GetTiles() { return m_Rotations[m_CurrentRotation]; }

	bool IsFalling() { return m_IsFalling; }

	void Render();
};

