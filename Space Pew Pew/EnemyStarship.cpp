#include "EnemyStarship.h"

EnemyStarship::EnemyStarship(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
	float width, float height, float movementSpeed,
	float shootingSpeed, float turnSpeed, unsigned int typeID, irrklang::ISoundEngine* soundEngine)
	: Starship(mesh, position, scale, modelLocation, shaderID, width, height, movementSpeed, nullptr, shootingSpeed)
{
	m_TypeID = typeID;

	mT_Enemy5Default = std::shared_ptr<Texture>(new Texture("Textures/Enemy5.png"));

	if (typeID == 1)
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/enemy1.png"));
	else if (typeID == 6)
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/Enemy2.png"));
	else if (typeID == 7)
		m_Texture = std::shared_ptr<Texture>(new Texture("Textures/Enemy3.png"));
	else if (typeID == 8)
		m_Texture = mT_Enemy5Default;

	if (m_TypeID == 7)
		m_Health = 2;

	if (m_TypeID == 8)
		m_Health = 5;

	m_ChangeInDirection = glm::vec2(0.01f, 0.0f);

	if (rand() % 2 == 1)
		m_ChangeInDirection.x = -m_ChangeInDirection.x;
		
	m_Direction = glm::vec2(0.0f, -1.0f);

	if (m_TypeID == 8)
	{
		m_Direction = rand() % 10 > 5 ? glm::normalize(glm::vec2(2.0f, -2.0f)) : glm::normalize(glm::vec2(-2.0f, -2.0f));
	}

	m_TurnSpeed = turnSpeed;

	m_SoundEngine = soundEngine;

	m_Sound = m_SoundEngine->play2D("Audio/falling2.wav", false, false, true);

	m_DeadTime += 0.5 / ((rand() % 2) + 1);
	m_LoopTime *= 3 / ((rand() % 2) + 1);

	mT_Explosion1 = std::shared_ptr<Texture>(new Texture("Textures/explosion1.png"));
	mT_Explosion2 = std::shared_ptr<Texture>(new Texture("Textures/explosion2.png"));
	mT_Enemy5Damage = std::shared_ptr<Texture>(new Texture("Textures/Enemy5_Damaged.png"));
}

void EnemyStarship::Update(GLfloat& deltaTime)
{
	// Set velocity and incriment lazer stopwatchd
	m_LazerStopwatch += deltaTime;
	m_MovementStopwatch += deltaTime;
	float velocity = m_MovementSpeed * deltaTime;

	// Movement back and forth
	m_Position += m_Direction * velocity;

	if (!m_IsDead && !m_IsLooping && (m_TypeID == 1 || m_TypeID == 6 || m_TypeID == 7))
		m_Direction.x += m_ChangeInDirection.x * velocity;

	if (m_MovementStopwatch > m_TurnSpeed && !m_IsLooping)
	{
		m_MovementStopwatch = 0.0f;
		m_ChangeInDirection.x = 1.4f * -m_ChangeInDirection.x;
		m_TurnSpeed *= 1.8f;
	}

	if (m_IsLooping && !m_IsDead)
	{
		// To simulate gravity and orbiting around a point (loop point)
		m_Direction += glm::normalize(m_LoopPoint - m_Position) * (float)(1.0f / (glm::distance(m_LoopPoint, m_Position) + 0.00001));
	}

	// Enemy 3 behavior
	if (m_TypeID == 7)
		m_LoopTimer -= deltaTime;

	if (m_TypeID == 7 && m_LoopTimer < 0.0f && !m_IsLooping && !m_DontLoop && !m_IsDead)
	{
		m_IsLooping = true;
		m_LoopTimer = m_InLoopTime;

		m_LoopPoint = glm::vec2(0.0f, 0.0f);
	}

	if (m_LoopTimer < 0.0f && m_IsLooping && !m_IsDead)
	{
		m_IsLooping = false;
		m_DontLoop = true;

		m_Direction.y = -1.0f;
		m_Direction.x = m_Direction.x / 10;
		m_ChangeInDirection.x = m_ChangeInDirection.x / 10;
	}

	if (m_TypeID == 7 && !m_IsDead && m_DontLoop)
	{
		m_ChangeInDirection += glm::vec2(0.0f, -1.0f);
	}

	if (m_LazerStopwatch > m_ShootingSpeed && !m_IsDead)
	{
		// Add a new lazer to list when they press spacebar and the stopwatch is greater then time specified
		m_ShootLazer = true;
		m_LazerStopwatch = 0.0f;
	}

	// If starship is outside window (vertically) delete it

	if (m_Position.y > (float)m_Height / 2)
		m_Delete = true;
	
	if (m_Position.y < -(float)m_Height / 2)
		m_Delete = true;

	if (m_Position.x > (float)m_Width / 2 && !m_IsLooping)
		m_Position.x = -(float)m_Width / 2;
	
	if (m_Position.x < -(float)m_Width / 2 && !m_IsLooping)
		m_Position.x = (float)m_Width / 2;

	if (m_IsDead)
	{
		if (m_Health > 1)
		{
			m_Health--;
			m_IsDead = false;

			if (m_TypeID == 7)
				m_Texture = std::shared_ptr<Texture>(new Texture("Textures/Enemy4.png"));

			if (m_TypeID == 8)
			{
				m_DamageAnimation = true;
				m_DamageTimer = m_DamageTime;
				m_Texture = mT_Enemy5Damage;
			}
		}

		m_DeadStopwatch += deltaTime;
		m_TextureSwitchTimer -= deltaTime;
	}

	if (m_TypeID == 8)
		m_DamageTimer -= deltaTime;

	if (m_DamageAnimation && m_DamageTimer < 0.0f)
	{
		m_DamageAnimation = false;
		m_Texture = mT_Enemy5Default;
	}

	if (m_IsDead && !m_StoppedSound)
	{
		m_Sound->setIsPaused();
		m_StoppedSound = true;
	}

	if (m_IsDead && m_TextureSwitchTimer < 0.0f)
	{
		m_TextureSwitchTimer = 0.3f;
		if (m_TextureToggle)
			m_Texture = mT_Explosion1;
		else if (!m_TextureToggle)
			m_Texture = mT_Explosion2;

		m_TextureToggle = !m_TextureToggle;
	}

	if (m_IsDead && m_DeadStopwatch > m_DeadTime)
	{
		m_Delete = true;
	}
}
