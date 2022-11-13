#pragma once

#include "Rectangle.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <memory>
#include <vector>
#include <list>

#include "Texture.h"

class Object
{
protected:
	std::shared_ptr<Mesh> m_Mesh;

	glm::vec2 m_Position;
	glm::vec2 m_Scale;

	GLuint m_ModelLocation;
	glm::mat4 m_ModelMatrix;

	GLuint m_ShaderID;

	std::shared_ptr<Texture> m_Texture;

	bool m_Delete = false;

	unsigned int m_TypeID;

	bool m_Render = true;

	float m_Width, m_Height;

	bool* m_Keys;

	GLenum m_KeyUp, m_KeyDown, m_KeyLeft, m_KeyRight, m_KeyShoot;

	float m_MovementSpeed;

public:
	Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
		float width, float height, float movementSpeed, bool* keys,
		const char* textureFileLocation, unsigned int typeID);

	Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
		float width, float height, float movementSpeed, bool* keys, unsigned int typeID);


	Object(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
		float width, float height, float movementSpeed, bool* keys,
		const char* textureFileLocation, unsigned int typeID, bool render);

	virtual void Update(GLfloat& deltaTime) {};
	virtual std::list<glm::vec2> GetLazerPositions() { return std::list<glm::vec2>(); };

	virtual void Render();

 	glm::vec2 GetPosition() { return m_Position; }

	glm::vec2 GetScale() { return m_Scale; }
	void SetScale(glm::vec2 scale) { m_Scale = scale; }

	void Move(glm::vec2 trajectory) { m_Position += trajectory; }
	void SetPosition(glm::vec2 position) { m_Position = position; }
	
	void SetDelete(bool shouldDelete) { m_Delete = shouldDelete; }
	bool GetDelete() { return m_Delete; }

	virtual void SetIsDead(bool dead) {};

	virtual bool GetInvincible() { return false; }
	virtual void SetInvincible(bool invincible) { }

	unsigned int GetTypeID() { return m_TypeID; }

	virtual bool GetShootLazer() { return false; }
	virtual void SetShootLazer(bool shootLazer) {}

	void SetRender(bool render) { m_Render = render; }
	bool GetRender() { return m_Render; }

	float GetMovementSpeed() { return m_MovementSpeed; }
	void SetMovementSpeed(float movementSpeed) { m_MovementSpeed = movementSpeed; }

	virtual bool GetIsDead() { return false; }

	virtual float GetShootSpeed() { return 0.0f; }
	virtual void SetShootSpeed(float shootSpeed) {}

	virtual unsigned int GetHealth() { return 0; }
};

