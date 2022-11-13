#pragma once
#include "Object.h"
class Powerup :
    public Object
{
public:
    Powerup(std::shared_ptr<Mesh> mesh, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
        float width, float height, float movementSpeed, unsigned int typeID);

    void Update(float& deltaTime);

private:
    float m_SwitchTime;
    float m_TextureSwitchTimer;

    bool m_TextureToggle = false;
};

