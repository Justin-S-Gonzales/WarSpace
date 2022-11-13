#include "Powerup.h"

Powerup::Powerup(std::shared_ptr<Mesh> mesh, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, float width, float height, float movementSpeed, unsigned int typeID)
	: Object(mesh, glm::vec2(0.0f, 0.0f), scale, modelLocation, shaderID, width, height, movementSpeed, nullptr, typeID)
{
	if (typeID == 1001)
	{
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/1Up_1.png"));
	}

	if (typeID == 1002)
	{
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/PowUp_1.png"));
	}

	if (typeID == 1003)
	{
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/SPDUP_1.png"));
	}

	if (typeID == 1004)
	{
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/ShieldPickup.png"));
	}

	if (typeID == 1005)
	{
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/screenClear.png"));
	}
	
	m_Position = glm::vec2((rand() % ((int)width / 2)), height / 2);
	if (rand() % 2 == 1)
	{
		m_Position.x = -m_Position.x;
	}

	m_SwitchTime = (2.0f / ((rand() % 7) + 1)) + 0.3f;
	m_TextureSwitchTimer = m_SwitchTime;

	if (rand() % 2 == 1)
	{
		m_MovementSpeed *= 1.3;
	}
}

void Powerup::Update(float& deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;

	m_TextureSwitchTimer -= deltaTime;

	if (m_TextureSwitchTimer < 0.0f)
	{
		m_TextureSwitchTimer = m_SwitchTime;

		if (m_TypeID == 1001)
		{
			if (m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/1Up_1.png"));
			else if (!m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/1Up_2.png"));
		}

		if (m_TypeID == 1002)
		{
			if (m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/PowUp_1.png"));
			else if (!m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/PowUp_2.png"));
		}

		if (m_TypeID == 1003)
		{
			if (m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/SPDUP_1.png"));
			else if (!m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/SPDUP_2.png"));
		}

		if (m_TypeID == 1004)
		{
			if (m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/ShieldPickup.png"));
			else if (!m_TextureToggle)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/ShieldPickup_2.png"));
		}

		m_TextureToggle = !m_TextureToggle;
	}

	m_Position.y -= velocity;

	if (m_Position.y < -(float)m_Height / 2)
		m_Delete = true;
}


