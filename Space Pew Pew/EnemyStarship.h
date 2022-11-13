#pragma once
#include "Starship.h"

#include <irrKlang.h>

class EnemyStarship :
    public Starship
{
public:
    EnemyStarship(std::shared_ptr<Mesh> mesh, glm::vec2 position, glm::vec2 scale, GLuint modelLocation, GLuint shaderID,
        float width, float height, float movementSpeed,
        float shootingSpeed, float turnSpeed, unsigned int typeID, irrklang::ISoundEngine* soundEngine);

    void Update(GLfloat& deltaTime);

    void Start() { m_LazerStopwatch = 0.0f; }

    unsigned int GetHealth() { return m_Health; }

    ~EnemyStarship()
    {
        m_Sound->drop();
    }

private:
    float m_MovementStopwatch;
    glm::vec2 m_Direction;
    glm::vec2 m_ChangeInDirection;
    float m_TurnSpeed;

    irrklang::ISound* m_Sound;

    irrklang::ISoundEngine* m_SoundEngine;

    bool m_StoppedSound = false;

    float m_LoopTime = 1.3f;
    float m_InLoopTime = m_LoopTime * 3.0f;
    float m_LoopTimer = m_LoopTime;

    glm::vec2 m_LoopPoint = glm::vec2(0.0f, 0.0f);
    bool m_IsLooping = false;
    bool m_DontLoop = false;

    unsigned int m_Health = 1;

    std::shared_ptr<Texture> mT_Enemy5Default;
    std::shared_ptr<Texture> mT_Enemy5Damage;

    bool m_DamageAnimation;
    float m_DamageTime = 0.2f;
    float m_DamageTimer;
};

