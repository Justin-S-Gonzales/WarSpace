#pragma once
#include "Object.h"
#include "Lazer.h"

#include <GLFW/glfw3.h>

#include <list>
#include <memory>

class Starship :
    public Object
{
public:
    Starship(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
        float width, float height, float movementSpeed, bool* keys,
        float shootingSpeed);

    void Update(GLfloat& deltaTime);

    void SetIsDead(bool isDead) { m_IsDead = isDead; /*std::cout << "Is Dead" << std::endl;*/ }

    bool GetInvincible() { return m_Invincible; }
    virtual void SetInvincible(bool invincible) { m_Invincible = invincible; }

    bool GetShootLazer() { return m_ShootLazer; }
    void SetShootLazer(bool shootLazer) { m_ShootLazer = shootLazer; }

    bool GetGameOver() { return m_GameOver; }

    bool GetIsDead() { return m_IsDead; }

    float GetShootSpeed() { return m_ShootingSpeed; }
    void SetShootSpeed(float shootSpeed) { m_ShootingSpeed = shootSpeed; }

protected:

    GLfloat m_LazerStopwatch;
    float m_ShootingSpeed;

    bool m_ShootLazer = false;

    //std::list<std::shared_ptr<Lazer>> m_Lazers;

    bool m_IsDead = false;
    float m_DeadStopwatch = 0.0f;
    float m_DeadTime = 1.0f;
    float m_TextureSwitchTimer = 0.3f;
    bool m_TextureToggle = false;

    bool m_Invincible = false;
    float m_InvincibleTimer = 3.0f;

    bool m_GameOver = false;

    bool m_TurningLeft = false;
    bool m_TurningRight = false;

    std::shared_ptr<Texture> mT_Default;
    std::shared_ptr<Texture> mT_TurningLeft;
    std::shared_ptr<Texture> mT_TurningRight;
    std::shared_ptr<Texture> mT_Shield;
    std::shared_ptr<Texture> mT_Explosion1;
    std::shared_ptr<Texture> mT_Explosion2;
};

