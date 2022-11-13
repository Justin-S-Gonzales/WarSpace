#include "Rectangle.h"

Rectangle::Rectangle()
{
	// Mesh
	float vertices[] = {
		// positions  // texture coords
		 0.5f,  0.5f, 1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,   
		1, 2, 3
	};

	CreateMesh(indices, sizeof(indices), vertices, sizeof(vertices));
}
