#include "Object.h"
Object::Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
	float width, float height, float movementSpeed, bool* keys,
	const char* textureFileLocation, unsigned int typeID)
{
	m_Mesh = mesh;
	m_Position = position;
	m_Scale = scale;
	m_ModelLocation = modelLocation;
	m_ModelMatrix = glm::mat4(1.0f);

	m_ShaderID = shaderID;

	m_Width = width;
	m_Height = height;
	m_MovementSpeed = movementSpeed;
	m_Keys = keys;

	m_Texture = std::shared_ptr<Texture>(new Texture(textureFileLocation));

	m_TypeID = typeID;
}

Object::Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, float width, float height, float movementSpeed, bool* keys, unsigned int typeID)
{

	m_Mesh = mesh;
	m_Position = position;
	m_Scale = scale;
	m_ModelLocation = modelLocation;
	m_ModelMatrix = glm::mat4(1.0f);

	m_ShaderID = shaderID;

	m_Width = width;
	m_Height = height;
	m_MovementSpeed = movementSpeed;
	m_Keys = keys;

	m_TypeID = typeID;
}

Object::Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, 
	float width, float height, float movementSpeed, bool* keys, const char* textureFileLocation, unsigned int typeID, bool render)
	: Object(mesh, position, scale, modelLocation, shaderID, width, height, movementSpeed, keys, textureFileLocation, typeID)
{
	m_Render = render;
}

void Object::Render()
{
	m_Texture->UseTexture();

	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(m_Position, 0.0f));
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Scale, 0.0f));

	glUniformMatrix4fv(m_ModelLocation, 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));

	if (m_Render)
		m_Mesh->Render(); 
}
