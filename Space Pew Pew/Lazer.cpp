#include "Lazer.h"

Lazer::Lazer(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
	float width, float height, float movementSpeed, unsigned int typeID, glm::vec2 direction)
	: Object(mesh, position, scale, modelLocation, shaderID, width, height, movementSpeed, nullptr, typeID)
{
	switch (m_TypeID)
	{
	case 5001:
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/lazer3.png"));
		m_Direction = glm::vec2(0.0f, 1.0f);
		break;
	case 5002:
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/lazer4.png"));
		m_Direction = glm::vec2(0.0f, -1.0f);
		break;
	case 5003:
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/laser3.png"));
		m_Direction = direction;
		break;
	}
}

void Lazer::Update(GLfloat& deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;

	m_Position += m_Direction * velocity;

	if (m_Position.y > (float)m_Height / 2)
		m_Delete = true;

	if (m_Position.y < -(float)m_Height / 2)
		m_Delete = true;
}