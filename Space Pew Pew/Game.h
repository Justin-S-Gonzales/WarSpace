#pragma once

#include <list>
#include <memory>
#include <algorithm>

#include "Object.h"
#include "Starship.h"
#include "EnemyStarship.h"
#include "Mesh.h"
#include "Rectangle.h"
#include "Star.h"
#include "Powerup.h"

#include "Shader.h"
#include "TextRenderer.h"

#include <irrKlang.h>

#include <fstream>

class Game : public Object
{
private:
	// Objects
	std::list<std::unique_ptr<Object>> m_Objects;

	// Shaders
	std::shared_ptr<Shader> m_MainShader;
	std::shared_ptr<Shader> m_TextShader;

	// Text
	TextRenderer m_TextRenderer;

	// Stars
	float m_StarTimer;
	float m_StarInterval;

	// TitleScreen
	bool m_HasStartedTitleScreenSong = false;
	irrklang::ISound* m_TitleSong;

	float m_TitleTimer = 1.0f;
	bool m_TitleScreen = true;
	float m_PromptFlashTime = 0.6f;
	float m_PromptFlashTimer = m_PromptFlashTime;

	// Levels
	unsigned int m_Level = 0;

	float m_InstructionTime = 5.0f;
	float m_InstructionTimer = m_InstructionTime;
	
	bool m_ShownLevelCounter = false;

	float m_EnemySpawnTime;
	float m_EnemySpawnTimer;

	float m_LevelTimer;

	unsigned int m_LastLevel = 10;

	// Game over and win screens
	bool m_GameOver = false;
	float m_GameOverTime = 1.0f;
	float m_GameOverTimer = m_GameOverTime;

	bool m_YouWon = false;
	float m_YouWonTime = 1.0f;
	float m_YouWonTimer = m_YouWonTime;

	// Powerups
	bool m_Powerups = true;

	unsigned int m_PlayerLaserLevel = 0;
	unsigned int m_PlayerSpeedLevel = 0;
	unsigned int m_PlayerShieldLevel = 0;

	unsigned int m_PlayerLaserMax = 2;
	unsigned int m_PlayerSpeedMax = 2;
	unsigned int m_PlayerShieldMax = 2;

	float m_PowerUpSpawnTime = 9.0f;
	float m_PowerUpSpawnTimer = m_PowerUpSpawnTime;

	bool m_1UpSpawned = false;

	bool m_ClearingScreen = false;
	float m_ClearingScreenTime = 0.8f;
	float m_ClearingScreenTimer;

	// Shield
	bool m_ShieldDestroyed = false;
	float m_ShieldDeathSpeed = 2.5f;
	float m_ShieldDeathTimer = m_ShieldDeathSpeed;
	float m_ShieldSwitchToggle = false;
	float m_ShieldSwitchSpeed = 0.2f;
	float m_ShieldSwitchTimer = m_ShieldSwitchSpeed;
	
	bool m_ShieldDamageAnimation = false;
	float m_ShieldDamageTime = 0.4f;
	float m_ShieldDamageTimer;

	// Sound
	irrklang::ISoundEngine* m_SoundEngine;

	float m_MuteTime = 2.0f;
	float m_MuteTimer = 0.0f;
	bool m_Muted = false;

	float m_PowerupToggleTime = 0.5f;
	float m_PowerupToggleTimer = m_PowerupToggleTime;

	// Score
	unsigned int m_Score = 0;
	std::string m_ScoreString;
	unsigned int m_ScoreStringLen = 8;

	struct Score
	{
		unsigned int score;
		std::string name;
	};
	
	std::vector<Score> m_Scores;
	unsigned int m_HighScore = 0;
	std::string m_HighScoreString;

	Score m_PlayerScore;

	bool m_HasTypedName = false;
	bool m_UpdatedScoreList = false;

	bool m_ShowEnterName = true;
	unsigned int m_MaxNameLength = 15;

	float m_ScrollSpeed = 0.1f;
	float m_ScrollTimer = m_ScrollSpeed;

	unsigned int m_FirstScore = 0;

	bool m_Canceled = false;

	bool m_IsTyping = false;

	bool m_DebugMode = false;

	// Lasers
	unsigned int m_NumOfPlayerLazers;

	// Lives
	glm::vec2 m_OriginalLifePos = glm::vec2((m_Width / 1.7f) - 100.0f, -m_Height / 2.3f);

	unsigned int m_Lives = 3;
	unsigned int m_GameStartLives = 3;
	const unsigned int m_MaxLives = 5;

	/* PRIVATE METHODS */
	void UpdateTitleScreen(float& deltaTime);

	void IncrLevel();

	void CountPlayerLazers();

	void UpdateLevelDisplay(GLfloat& deltaTime);

	void UpdateGameLevel(GLfloat& deltaTime);

	void DeleteEntities();

	void CreateHighScoreString();

	static bool CompareByScoreNumber(const Score& a, const Score& b);

	void AddToScores(float& deltaTime);

	void DisplayScores(float& deltaTime);

	void CheatCode(float& deltaTime);

	void ClearKeys();

	bool PressedButton();
	void PressedButtonPrompt(float& deltaTime);

	void ResetPowerups(bool all);

	void UpdateShield(Object& player);

	void EndGame();
public:
	Game(std::shared_ptr<Mesh> rect, GLuint uModel, GLuint shaderID, bool* keys, float width, float height, 
		std::shared_ptr<Shader> shader, std::shared_ptr<Shader> textShader);
	  
	void Update(GLfloat deltaTime);
};

