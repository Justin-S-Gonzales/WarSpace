#include "Starship.h"

Starship::Starship(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
	float width, float height, float movementSpeed, bool* keys,
	float shootingSpeed)
	: Object(mesh, position, scale, modelLocation, shaderID, width, height, movementSpeed, keys, "Textures/starship.png", 0)
{
	m_ShootingSpeed = shootingSpeed;

	m_KeyUp     = GLFW_KEY_W;
	m_KeyDown   = GLFW_KEY_S;
	m_KeyLeft   = GLFW_KEY_A;
	m_KeyRight  = GLFW_KEY_D;
	m_KeyShoot  = GLFW_KEY_SPACE;

	m_LazerStopwatch = 0.0f;

	mT_Default = std::shared_ptr<Texture>(new Texture("Textures/starship.png"));
	mT_TurningLeft = std::shared_ptr<Texture>(new Texture("Textures/starshipTurningLeft.png"));
	mT_TurningRight = std::shared_ptr<Texture>(new Texture("Textures/starshipTurningRight.png"));

	mT_Shield = std::shared_ptr<Texture>(new Texture("Textures/starshipShield.png"));

	mT_Explosion1 = std::shared_ptr<Texture>(new Texture("Textures/explosion3.png"));
	mT_Explosion2 = std::shared_ptr<Texture>(new Texture("Textures/explosion4.png"));
}

void Starship::Update(GLfloat& deltaTime)
{
	// Set velocity and incriment lazer stopwatch
	m_LazerStopwatch += deltaTime;
	float velocity = m_MovementSpeed * deltaTime;

	m_TurningLeft = false;
	m_TurningRight = false;

	// Set reactions to controlls
	if ((m_Keys[m_KeyLeft] || m_Keys[GLFW_KEY_LEFT]) && !m_IsDead)
	{
		m_Position += glm::vec2(-1.0f, 0.0f) * velocity;
		m_TurningLeft = true;
		m_TurningRight = false;
	}
	
	else if ((m_Keys[m_KeyRight] || m_Keys[GLFW_KEY_RIGHT]) && !m_IsDead)
	{
		m_Position += glm::vec2(1.0f, 0.0f) * velocity;
		m_TurningRight = true;
		m_TurningLeft = false;
	}
	
	if ((m_Keys[m_KeyUp] || m_Keys[GLFW_KEY_UP]) && !m_IsDead)
	{
		m_Position += glm::vec2(0.0f, 1.0f) * velocity;
	}
	
	else if ((m_Keys[m_KeyDown] || m_Keys[GLFW_KEY_DOWN]) && !m_IsDead)
	{
		m_Position += glm::vec2(0.0f, -1.0f) * velocity;
	}
	
	if (!(m_Keys[m_KeyLeft] || m_Keys[GLFW_KEY_LEFT]) && !(m_Keys[m_KeyRight] || m_Keys[GLFW_KEY_RIGHT]) && !m_IsDead)
	{
		m_TurningLeft = false;
		m_TurningRight = false;
	}

	if (m_TurningLeft && !m_IsDead && !m_Invincible)
	{
		m_Texture = mT_TurningLeft;
	}
	if (m_TurningRight && !m_IsDead && !m_Invincible)
	{
		m_Texture = mT_TurningRight;
	}
	if (((!m_TurningRight && !m_TurningLeft) || (m_TurningRight && m_TurningLeft)) && !m_IsDead && !m_Invincible)
	{
		m_Texture = mT_Default;
	}
	if (m_Keys[m_KeyShoot] && m_LazerStopwatch > m_ShootingSpeed && !m_IsDead)
	{
		m_ShootLazer = true;
		m_LazerStopwatch = 0.0f;
	}

	if (!m_Keys[m_KeyShoot])
	{
		m_ShootLazer = false;
	}

	// If starship is outside window, stop it
	if (m_Position.x >  (float)m_Width / 2)
		m_Position.x =  -(float)m_Width / 2;

	if (m_Position.x < -(float)m_Width / 2)
		m_Position.x = (float)m_Width / 2;

	if (m_Position.y >  m_Height / 3.0f)
		m_Position.y = m_Height / 3.0f;

	if (m_Position.y < -(float)m_Height / 2)
		m_Position.y = -(float)m_Height / 2;

	if (m_IsDead && !m_Invincible)
	{
		m_DeadStopwatch += deltaTime;
		m_TextureSwitchTimer -= deltaTime;
	}

	if (m_IsDead && m_TextureSwitchTimer < 0.0f && !m_Invincible)
	{
		m_TextureSwitchTimer = 0.3f;
		if (m_TextureToggle)
			m_Texture = mT_Explosion1;
		else if (!m_TextureToggle)
			m_Texture = mT_Explosion2;

		m_TextureToggle = !m_TextureToggle;
	}

	if (m_IsDead && m_DeadStopwatch > m_DeadTime && !m_Invincible)
	{
		//std::cout << "Lives: " << m_Lives << std::endl;
		m_IsDead = false;
		m_Texture = mT_Default;
		m_Position = glm::vec2(0.0f, -m_Height / 2 + m_Scale.x);
		m_InvincibleTimer = 3.0f;
		m_Invincible = true;
		m_DeadStopwatch = 0.0f;
	}

	if (m_Invincible)
	{
		m_InvincibleTimer -= deltaTime;
		m_IsDead = false;

		m_TextureSwitchTimer -= deltaTime;
	}

	if (m_TextureSwitchTimer < 0.0f && m_Invincible)
	{
		m_TextureSwitchTimer = 0.6f;
		if (m_TextureToggle)
			m_Texture = mT_Default;
		else if (!m_TextureToggle)
			m_Texture = mT_Shield;

		m_TextureToggle = !m_TextureToggle;
	}

	if (m_Invincible && m_InvincibleTimer <= 0.0f)
	{
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/Starship.png"));

		m_InvincibleTimer = 3.0f;
		m_Invincible = false;
	}

}