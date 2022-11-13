#include "Star.h"

Star::Star(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale,
	GLuint modelLocation, GLuint shaderID, int windowWidth, int windowHeight, float movementSpeed, bool spawnAnywhere)
	: Object(mesh, position, scale, modelLocation, shaderID, windowWidth, windowHeight, movementSpeed, nullptr, "Textures/Star1.png", 2)
{
	if (spawnAnywhere)
	{
		m_Position = glm::vec2((rand() % (int) windowWidth) / 2.0f, (rand() % (int) windowHeight) / 2.0f);

		if (rand() % 4 == 1)
		{
			m_Position.x = -m_Position.x;
		}
		else if (rand() % 4 == 2)
		{
			m_Position.y = -m_Position.y;
		}
		else if (rand() % 4 == 3)
		{
			m_Position.x = -m_Position.x;
			m_Position.y = -m_Position.y;
		}
	}
	else
	{
		m_Position = glm::vec2((rand() % (windowWidth / 2)), windowHeight / 2);
		if (rand() % 2 == 1)
		{
			m_Position.x = -m_Position.x;
		}
	}

	m_BlinkTime = (2.0f / ((rand() % 7) + 1)) + 0.3f;
	m_TextureSwitchTimer = m_BlinkTime;

	if (rand() % 2 == 1)
	{
		m_MovementSpeed *= 1.3;
	}
}

void Star::Update(GLfloat& deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;

	m_TextureSwitchTimer -= deltaTime;

	if (m_TextureSwitchTimer < 0.0f)
	{
		m_TextureSwitchTimer = m_BlinkTime;

		if (m_TextureToggle)
			m_Texture = std::shared_ptr<Texture>(new Texture("Textures/Star1.png"));
		else if (!m_TextureToggle)
			m_Texture = std::shared_ptr<Texture>(new Texture("Textures/Star2.png"));

		m_TextureToggle = !m_TextureToggle;
	}

	m_Position.y -= velocity;

	if (m_Position.y < -(float)m_Height / 2)
		m_Delete = true;
}
