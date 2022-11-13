#pragma once
#include "Object.h"
class Star :
    public Object
{
public:
    Star(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID, 
        int windowWidth, int windowHeight, float movementSpeed, bool spawnAnywhere);

    void Update(GLfloat& deltaTime);

private:
    float m_BlinkTime;
    float m_TextureSwitchTimer; 

    bool m_TextureToggle = false;
};

