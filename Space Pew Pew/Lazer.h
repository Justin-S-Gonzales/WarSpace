#pragma once
#include "Object.h"
class Lazer :
    public Object
{
public:
    Lazer(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
        float width, float height, float movementSpeed, unsigned int typeID, glm::vec2 direction = glm::vec2(0.0f));

    void Update(GLfloat& deltaTime);

private:

    glm::vec2 m_Direction;

};

