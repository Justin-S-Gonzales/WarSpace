#include "Game.h"

void Game::UpdateTitleScreen(float& deltaTime)
{
	if (m_TitleScreen)
	{
		if (!m_HasStartedTitleScreenSong)
		{
			m_HasStartedTitleScreenSong = true;
			m_TitleSong = m_SoundEngine->play2D("Audio/spacebattle.wav", true, false, true);

			DeleteEntities();
		}

		m_PowerupToggleTimer -= deltaTime;
		// To display score with S
		if (!m_Keys[GLFW_KEY_S])
		{
			m_TextShader->UseShader();
			m_TextRenderer.RenderText(*m_TextShader, "HOLD S TO SHOW SCORES", m_Width * (1.0f / 10.0f), 30.0f, 0.4, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
			m_TextRenderer.RenderText(*m_TextShader, "PRESS M TO MUTE", m_Width * (6.0f / 10.0f), 30.0f, 0.4, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
			m_TextRenderer.RenderText(*m_TextShader, "PRESS P TO TOGGLE POWERUPS", m_Width * (1.0f / 10.0f), 60.0f, 0.4, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
			m_TextRenderer.RenderText(*m_TextShader, "POWERUPS: ", m_Width * (7.0f / 10.0f), 60.0f, 0.4, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);

			if (m_PowerupToggleTimer <= 0.0f)
			{
				if (m_Keys[GLFW_KEY_P])
				{
					m_PowerupToggleTimer = m_PowerupToggleTime;
					m_Powerups = !m_Powerups;
				}
			}
			
			if (m_Powerups)
			{
				m_TextRenderer.RenderText(*m_TextShader, "ON", m_Width * (9.0f / 10.0f), 60.0f, 0.4, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
			}
			else
			{
				m_TextRenderer.RenderText(*m_TextShader, "OFF", m_Width * (9.0f / 10.0f), 60.0f, 0.4, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
			}
		}

		if (m_Keys[GLFW_KEY_S])
		{
			DisplayScores(deltaTime);

			for (auto& prompt : m_Objects)
			{
				if (prompt->GetTypeID() == 16)
				{
					prompt->SetRender(false);
				}
			}
		}

		for (auto& titleCard : m_Objects)
		{
			if (titleCard->GetTypeID() == 15)
			{
				titleCard->SetRender(true);
			}
		}

		m_TitleTimer -= deltaTime;
		m_PromptFlashTimer -= deltaTime;

		if (m_PromptFlashTimer <= 0.0f && !m_Keys[GLFW_KEY_S])
		{
			m_PromptFlashTimer = m_PromptFlashTime;

			for (auto& prompt : m_Objects)
			{
				if (prompt->GetTypeID() == 16)
				{
					prompt->SetRender(!prompt->GetRender());
				}
			}
		}

		bool pressedButton = PressedButton();

		// If they press any button
		if (pressedButton && m_TitleTimer <= 0.0f)
		{
			m_SoundEngine->play2D("Audio/pressStart.ogg", false);
			m_SoundEngine->play2D("Audio/gamestart.wav", false);

			// Add player to object list
			m_Objects.push_back(std::unique_ptr<Starship>(new Starship(m_Mesh, glm::vec2(0.0f, -m_Height / 2 + 100.0f), glm::vec2(100.0f, 100.0f),
				m_ModelLocation, m_ShaderID, m_Width, m_Height, 360.0f, m_Keys, 0.3f)));

			m_Objects.push_back(std::unique_ptr<Object>(new Object(m_Mesh, glm::vec2(0.0f), glm::vec2(115.0f, 115.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
				0.0f, nullptr, "Textures/Shield.png", 3001, false)));

			m_Objects.push_back(std::unique_ptr<Object>(new Object(m_Mesh, glm::vec2(0.0f), glm::vec2(115.0f, 115.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
				0.0f, nullptr, "Textures/BrokenShield.png", 3002, false)));

			m_Objects.push_back(std::unique_ptr<Object>(new Object(m_Mesh, glm::vec2(0.0f), glm::vec2(115.0f, 115.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
				0.0f, nullptr, "Textures/ShieldDamaged.png", 3003, false)));

			m_TitleScreen = false;
			m_HasStartedTitleScreenSong = false;
			m_TitleSong->setIsPaused();
			m_TitleSong->drop();

			m_Score = 0;

			m_InstructionTimer = m_InstructionTime;

			IncrLevel();

			m_Lives = m_GameStartLives;
		}
	}
}

void Game::CountPlayerLazers()
{
	m_NumOfPlayerLazers = 0;
	for (auto& lazer : m_Objects)
	{
		if (lazer->GetTypeID() == 5001)
		{
			m_NumOfPlayerLazers++;
		}
	}
}

void Game::UpdateLevelDisplay(GLfloat& deltaTime)
{
	if (m_Level > 0 && !m_ShownLevelCounter)
	{
		m_PromptFlashTimer -= deltaTime;

		m_TextRenderer.RenderText(*m_TextShader, "LEVEL " + std::to_string(m_Level), m_Width / 2.4f, m_Height / 2.0f, 0.6f, glm::vec3(1.0f, 1.0f, 1.0f),
			m_Width, m_Height);

		if (m_PromptFlashTimer <= 0.0f)
		{
			m_ShownLevelCounter = true;
		}
	}
}

bool Game::CompareByScoreNumber(const Score& a, const Score& b)
{
	return a.score > b.score;
}

void Game::AddToScores(float& deltaTime)
{
	if (!m_HasTypedName && !m_Canceled)
	{
		// For mute function
		m_IsTyping = true;

		// For flashing effect for entering name
		m_PromptFlashTimer -= deltaTime;

		if (m_PromptFlashTimer < 0.0f)
		{
			m_ShowEnterName = !m_ShowEnterName;
			m_PromptFlashTimer = m_PromptFlashTime;
		}

		m_TextShader->UseShader();
		if (m_ShowEnterName)
		{
			m_TextRenderer.RenderText(*m_TextShader, "ENTER NAME:", m_Width * (1 / 10), m_Height / 2.0f + 50.0f, 0.6, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
			m_TextRenderer.RenderText(*m_TextShader, "(PRESS DELETE TO CANCEL)", m_Width * (1 / 10), m_Height / 2.0f, 0.3, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
		}

		m_TextRenderer.RenderText(*m_TextShader, m_PlayerScore.name, m_Width / 2.0f, m_Height / 2.0f + 50.0f, 0.6, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);

		for (int i = 65; i <= 90; i++)
		{
			if (m_Keys[i])
			{
				m_SoundEngine->play2D("Audio/TextEnter.wav");
				m_PlayerScore.name += (char)i;

				m_Keys[i] = false;
			}
		}

		if (m_Keys[GLFW_KEY_BACKSPACE])
		{
			m_SoundEngine->play2D("Audio/TextDelete.wav");

			if (m_PlayerScore.name.size() > 0)
				m_PlayerScore.name.pop_back();

			m_Keys[GLFW_KEY_BACKSPACE] = false;
		}

		if (m_Keys[GLFW_KEY_ENTER])
		{
			if (m_PlayerScore.name.size() > 0)
			{
				m_SoundEngine->play2D("Audio/pressStart.ogg", false);
				m_HasTypedName = true;

				m_Keys[GLFW_KEY_ENTER] = false;
			}
		}

		if (m_Keys[GLFW_KEY_DELETE])
		{
			m_SoundEngine->play2D("Audio/pressStart.ogg", false);
			m_Canceled = true;

			m_PlayerScore.name = "";
			m_PlayerScore.score = 0;

			m_IsTyping = false;
		}

		if (m_PlayerScore.name.size() > m_MaxNameLength)
		{
			m_PlayerScore.name.pop_back();
		}
	}
	else if (m_HasTypedName && !m_UpdatedScoreList && !m_Canceled)
	{
		m_IsTyping = false;
		m_PlayerScore.score = m_Score;

		// Add score to scores
		m_Scores.push_back(m_PlayerScore);

		std::sort(m_Scores.begin(), m_Scores.end(), &Game::CompareByScoreNumber);

		// Find scores with same name
		for (auto scorei = m_Scores.begin(); scorei != m_Scores.end(); scorei++)
		{
			for (auto scorej = m_Scores.begin(); scorej != m_Scores.end(); scorej++)
			{
				if (scorei == scorej)
					continue;

				if (scorei->name == scorej->name)
				{
					if (scorei->score > scorej->score)
					{
						m_Scores.erase(scorej);
						scorei = m_Scores.begin();
						scorej = m_Scores.begin();
					}
					else
					{
						m_Scores.erase(scorei);
						scorei = m_Scores.begin();
						scorej = m_Scores.begin();
					}
				}
			}
		}
		
		m_UpdatedScoreList = true;

		// Add scores to file
		std::ofstream file;

		// Delete contents of file first with std::ofstream::trunc
		file.open("scores.txt", std::ofstream::out | std::ofstream::trunc);

		file << m_Scores.size() << std::endl;

		for (int i = 0; i < m_Scores.size(); i++)
		{
			file << m_Scores[i].score << std::endl << m_Scores[i].name << std::endl;
		}

		file.close();

		m_PlayerScore.name = "";
		m_PlayerScore.score = 0;
	}
}

void Game::ClearKeys()
{
	{
		for (int i = 48; i < 314; i++)
		{
			m_Keys[i] = false;
		}

		for (int i = 262; i < 265; i++)
		{
			m_Keys[i] = false;
		}

		m_Keys[GLFW_KEY_BACKSPACE] = false;
		m_Keys[GLFW_KEY_ENTER] = false;
	}
}

bool Game::PressedButton()
{
	bool pressedButton = false;

	for (int i = 48; i < 90; i++)
	{
		if (m_Keys[i] && i != GLFW_KEY_S && i != GLFW_KEY_M && i != GLFW_KEY_P)
			pressedButton = true;
	}

	if (m_Keys[GLFW_KEY_SPACE])
		pressedButton = true;

	return pressedButton;
}

void Game::PressedButtonPrompt(float& deltaTime)
{
	if (m_HasTypedName)
	{
		m_YouWonTimer -= deltaTime;
		m_GameOverTimer -= deltaTime;
	}

	AddToScores(deltaTime);
	DisplayScores(deltaTime);

	bool pressedButton = PressedButton();

	if ((pressedButton && (m_YouWonTimer <= 0.0f || m_GameOverTime <= 0.0f))
		|| m_Canceled)
	{
		if (m_Canceled)
			m_Canceled = false;

		m_TitleTimer = 3.0f;
		m_TitleScreen = true;
		m_YouWon = false;
		m_GameOver = false;
		m_HasTypedName = false;
		m_UpdatedScoreList = false;

		for (auto& screen : m_Objects)
		{
			if (screen->GetTypeID() == 27)
			{
				screen->SetRender(false);
			}

			if (screen->GetTypeID() == 20)
			{
				screen->SetRender(false);
			}
		}
	}
}

Game::Game(std::shared_ptr<Mesh> rect, GLuint uModel, GLuint shaderID, bool* keys, float width, float height,
	std::shared_ptr<Shader> shader, std::shared_ptr<Shader> textShader)
	: Object(rect, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), uModel, shaderID, width, height, 0.0f, keys, 100)
{
	// Create sound engine
	m_SoundEngine = irrklang::createIrrKlangDevice();

	// Shaders
	m_MainShader = shader;
	m_TextShader = textShader;

	// Text renderer
	m_TextRenderer = TextRenderer("Fonts/uni0553-webfont.ttf");

	// Initial objects
	for (int i = 0; i < 50; i++) 
	{ 
		m_Objects.push_back(std::unique_ptr<Star>(new Star(rect, glm::vec2(0.0f, 0.0f), glm::vec2(50.0f, 50.0f), uModel, shaderID, width, height, 30.0f, true)));
	}

	m_Objects.push_back(std::unique_ptr<Object>(new Object(rect, glm::vec2(0.0f, m_Height / 4.0f), glm::vec2(300.0f, 300.0f), uModel, shaderID, m_Width, m_Height, 0, nullptr,
		"Textures/WarSpace.png", 15)));

	m_Objects.push_back(std::unique_ptr<Object>(new Object(rect, glm::vec2(0.0f, -m_Height / 4.0f), glm::vec2(130.0f, 130.0f), uModel, shaderID, m_Width, m_Height, 0, nullptr,
		"Textures/PressAnyButton.png", 16)));

	m_Objects.push_back(std::unique_ptr<Object>(new Object(rect, glm::vec2(0.0f, m_Height / 4.0f), glm::vec2(200.0f, 200.0f), uModel, shaderID, m_Width, m_Height, 0, nullptr,
		"Textures/WinScreen.png", 27, false)));

	m_Objects.push_back(std::unique_ptr<Object>(new Object(rect, glm::vec2(0.0f, m_Height / 4.0f), glm::vec2(130.0f, 130.0f), uModel, shaderID, width, height,
		0.0f, nullptr, "Textures/GameOver.png", 20, false)));

	m_Objects.push_back(std::unique_ptr<Object>(new Object(rect, m_OriginalLifePos, glm::vec2(130.0f, 130.0f), uModel, shaderID, width, height,
		0.0f, nullptr, "Textures/StarshipMini.png", 21, false)));

	m_StarInterval = 0.5f;
	m_StarTimer = m_StarInterval;
	m_Level = 0;

	m_KeyUp = GLFW_KEY_W;
	m_KeyDown = GLFW_KEY_S;
	m_KeyLeft = GLFW_KEY_A;
	m_KeyRight = GLFW_KEY_D;
	m_KeyShoot = GLFW_KEY_SPACE;

	ClearKeys();

	// Get scores from file
	std::ifstream file;
	file.open("scores.txt");

	int size;
	file >> size;

	m_Scores.resize(size);

	for (int i = 0; i < size; i++)
	{
		file >> m_Scores[i].score >> m_Scores[i].name;
	}

	file.close();
}

void Game::ResetPowerups(bool all)
{
	for (auto& player : m_Objects)
	{
		if (player->GetTypeID() == 0)
		{
			if (all)
			{
				m_PlayerLaserLevel = 0;

				if (m_PlayerSpeedLevel > 0)
				{
					player->SetMovementSpeed(player->GetMovementSpeed() - 150.0f * m_PlayerSpeedLevel);
					player->SetShootSpeed(player->GetShootSpeed() + 0.02 * m_PlayerSpeedLevel);
					m_PlayerSpeedLevel = 0;
				}

				m_PlayerShieldLevel = 0;
			}
			else
			{
				if (m_PlayerLaserLevel > 0)
				{
					m_PlayerLaserLevel--;
				}

				if (m_PlayerSpeedLevel > 0)
				{
					player->SetMovementSpeed(player->GetMovementSpeed() - 150.0f);
					player->SetShootSpeed(player->GetShootSpeed() + 0.02);
					m_PlayerSpeedLevel--;
				}
			}
		}
	}
}

void Game::CreateHighScoreString()
{
	m_ScoreString = std::to_string(m_Score);

	while (m_ScoreString.size() < m_ScoreStringLen)
	{
		m_ScoreString = "0" + m_ScoreString;
	}

	if (m_Scores.size() > 0)
	{
		if (m_Score > m_Scores[0].score)
		{
			m_HighScore = m_Score;
		}
		else
		{
			m_HighScore = m_Scores[0].score;
		}
	}
	else
	{
		m_HighScore = m_Score;
	}

	m_HighScoreString = std::to_string(m_HighScore);

	while (m_HighScoreString.size() < m_ScoreStringLen)
	{
		m_HighScoreString = "0" + m_HighScoreString;
	}
}

void Game::DisplayScores(float& deltaTime)
{
	m_ScrollTimer -= deltaTime;

	m_TextShader->UseShader();
	m_TextRenderer.RenderText(*m_TextShader, "SCORES:", m_Width * (2.0f / 5.0f), m_Height / 2.0f - 30.0f, 0.6f, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);

	m_TextRenderer.RenderText(*m_TextShader, "(PRESS ARROW KEYS TO SCROLL)", m_Width * (4.5f / 20.0f),
		m_Height / 2.0f - 55.0f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);

	if ((m_Keys[GLFW_KEY_DOWN]) && m_ScrollTimer < 0.0f)
	{
		m_ScrollTimer = m_ScrollSpeed;
		m_FirstScore++;
	}

	if ((m_Keys[GLFW_KEY_UP]) && m_FirstScore > 0 && m_ScrollTimer < 0.0f)
	{
		m_ScrollTimer = m_ScrollSpeed;
		m_FirstScore--;
	}

	int i = m_FirstScore;
	int j = 0;
	for (; i < m_Scores.size(); i++)
	{
		// Show
		m_TextRenderer.RenderText(*m_TextShader, std::to_string(i + 1), m_Width * ( 1.0f / 10.0f), (m_Height / 2.0f) - (90.0f + (j * 30.0f)), 0.6f, glm::vec3(1.0, 1.0f, 1.0f),
			m_Width, m_Height);

		// Show name
		m_TextRenderer.RenderText(*m_TextShader, m_Scores[i].name, m_Width * (2.0f / 10.0f), (m_Height / 2.0f) - (90.0f + (j * 30.0f)), 0.6f, glm::vec3(1.0, 1.0f, 1.0f),
			m_Width, m_Height);

		// Show score
		m_TextRenderer.RenderText(*m_TextShader, std::to_string(m_Scores[i].score), m_Width * (4.0f / 5.0f),
			(m_Height / 2.0f) - (90.0f + (j * 30.0f)),
			0.6f, glm::vec3(1.0, 1.0f, 1.0f),
			m_Width, m_Height);

		j++;
	}
}

void Game::CheatCode(float& deltaTime)
{
	deltaTime *= 40;
	for (auto& player : m_Objects)
	{
		if (player->GetTypeID() == 0)
		{
			player->SetInvincible(true);
		}
	}

	m_Lives++;
}

void Game::UpdateShield(Object& player)
{
	if (m_PlayerShieldLevel == 1)
	{
		m_SoundEngine->play2D("Audio/brokenShield.mp3", false);
		m_ShieldDestroyed = true;
		m_ShieldDeathTimer = m_ShieldDeathSpeed;
	}

	if (m_PlayerShieldLevel <= 0)
	{
		player.SetIsDead(true);

		ResetPowerups(false);

		if (m_Score > 200)
		{
			m_Score -= 200;
		}

		if (m_Lives > 0)
			m_Lives--;
	}

	if (m_PlayerShieldLevel >= 2)
	{
		m_SoundEngine->play2D("Audio/ShieldDeflected.mp3", false);

		m_ShieldDamageAnimation = true;
		m_ShieldDamageTimer = m_ShieldDamageTime;
	}

	if (m_PlayerShieldLevel >= 1)
	{
		player.SetInvincible(true);
		m_PlayerShieldLevel--;

		m_ShieldDamageAnimation = true;
		m_ShieldDamageTimer = m_ShieldDamageTime;
	}
}

void Game::EndGame()
{
	DeleteEntities();
	ResetPowerups(true);

	m_Level = 0;

	// Add score to scores
	m_HasTypedName = false;
	m_UpdatedScoreList = false;
}

void Game::Update(GLfloat deltaTime)
{
	// Cheat key (for debugging)
	if (m_Keys[GLFW_KEY_T] && m_DebugMode)
		CheatCode(deltaTime);

	// Update and render all game objects
	m_MainShader->UseShader();

	for (auto& object : m_Objects)
	{
		object->Update(deltaTime);
		object->Render();
	}

	// Erase all objects that need to be deleted
	for (auto object = m_Objects.begin(); object != m_Objects.end(); object++)
	{
		if (object->get()->GetDelete())
		{
			m_Objects.erase(object);
		}
	}

	// Render text
	CreateHighScoreString();
	m_TextShader->UseShader();

	if (!m_YouWon && !m_GameOver)
	{
		m_TextRenderer.RenderText(*m_TextShader, "HIGH SCORE", 20.0f, m_Height - 45.0f, 0.6f, glm::vec3(1.0, 1.0f, 1.0f),
			m_Width, m_Height);

		m_TextRenderer.RenderText(*m_TextShader, m_HighScoreString, 26.0f, m_Height - 90.0f, 0.6f, glm::vec3(1.0, 1.0f, 1.0f),
			m_Width, m_Height);
	}

	if (!m_TitleScreen && !m_GameOver && !m_YouWon)
	{
		m_TextRenderer.RenderText(*m_TextShader, "PLAYER 1", m_Width / 2 + 130.0f, m_Height - 45.0f, 0.6f, glm::vec3(1.0, 1.0f, 1.0f),
			m_Width, m_Height);

		m_TextRenderer.RenderText(*m_TextShader, m_ScoreString, m_Width / 2 + 120.0f, m_Height - 90.0f, 0.6f, glm::vec3(1.0, 1.0f, 1.0f),
			m_Width, m_Height);
	}

	m_InstructionTimer -= deltaTime;

	if (m_Level == 1 && m_InstructionTimer > 0.0f)
	{

		m_TextRenderer.RenderText(*m_TextShader, "MOVE WITH ASWD OR ARROW KEYS", m_Width / 2.0f - 230.0f, m_Height / 2.0f + 200.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
		m_TextRenderer.RenderText(*m_TextShader, "SHOOT LASERS WITH SPACEBAR", m_Width / 2.0f - 215.0f, m_Height / 2.0f + 170.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), m_Width, m_Height);
	}

	if (!m_GameOver && !m_YouWon && !m_TitleScreen && m_Powerups)
	{
		m_TextRenderer.RenderText(*m_TextShader, std::to_string(m_PlayerSpeedLevel), m_Width * (0.2f / 5.0f), m_Height * (0.23f / 5.0f),
			0.7f, glm::vec3(0.0f, 0.0f, 1.0f), m_Width, m_Height);

		m_TextRenderer.RenderText(*m_TextShader, std::to_string(m_PlayerLaserLevel), m_Width * (0.6f / 5.0f), m_Height * (0.23f / 5.0f),
			0.7f, glm::vec3(0.0f, 1.0f, 0.0f), m_Width, m_Height);

		m_TextRenderer.RenderText(*m_TextShader, std::to_string(m_PlayerShieldLevel), m_Width * (0.2f / 5.0f), m_Height * (0.43f / 5.0f),
			0.7f, glm::vec3(0.0f, 0.5f, 0.5f), m_Width, m_Height);
	}

	// Special keys

	m_MuteTimer -= deltaTime;
	if (m_Keys[GLFW_KEY_M] && m_MuteTimer <= 0.0f && !m_IsTyping)
	{
		m_MuteTimer = m_MuteTime;
		m_Muted = !m_Muted;
	}

	if (m_Muted)
	{
		m_SoundEngine->setSoundVolume(0);
	}
	else
	{
		m_SoundEngine->setSoundVolume(100);
	}

	// To create nice title screen
	UpdateTitleScreen(deltaTime);

	// Update level status
	UpdateLevelDisplay(deltaTime);

	UpdateGameLevel(deltaTime);

	// If player beats level 5
	if (m_Level == m_LastLevel + 1 && m_LevelTimer <= 0.0f)
	{
		m_SoundEngine->play2D("Audio/YouWon.wav", false);

		for (auto& youWonScreen : m_Objects)
		{
			if (youWonScreen->GetTypeID() == 27)
			{
				youWonScreen->SetRender(true);
			}
		}

		m_YouWon = true;
		m_YouWonTimer = m_YouWonTime;

		EndGame();
	}

	if (m_YouWon)
	{
		PressedButtonPrompt(deltaTime);
	}

	// Find player
	for (auto& player : m_Objects)
	{
		if (player->GetTypeID() == 0)
		{
			// Check if player has collided with enemies
			for (auto object = m_Objects.begin(); object != m_Objects.end(); object++)
			{
				if ((player->GetPosition().x > object->get()->GetPosition().x - (object->get()->GetScale().x / 3.0f) &&
					player->GetPosition().x < object->get()->GetPosition().x + (object->get()->GetScale().x / 3.0f) &&
					player->GetPosition().y > object->get()->GetPosition().y - (object->get()->GetScale().y / 3.0f) &&
					player->GetPosition().y < object->get()->GetPosition().y + (object->get()->GetScale().y / 3.0f)) &&
					player->GetPosition() != object->get()->GetPosition() &&
					!player->GetInvincible() &&
					(object->get()->GetTypeID() == 1 || 
						object->get()->GetTypeID() == 6 || 
						object->get()->GetTypeID() == 7 ||
						object->get()->GetTypeID() == 8) &&
					!player->GetIsDead())
				{
					m_SoundEngine->play2D("Audio/explosion2.wav", false);

					object->get()->SetIsDead(true);

					UpdateShield(*player);
				}
			}

			// Check if got powerup
			for (auto& powerup : m_Objects)
			{
				if (player->GetPosition().x > powerup->GetPosition().x - (powerup->GetScale().x / 2.0f) &&
					player->GetPosition().x < powerup->GetPosition().x + (powerup->GetScale().x / 2.0f) &&
					player->GetPosition().y > powerup->GetPosition().y - (powerup->GetScale().y / 2.0f) &&
					player->GetPosition().y < powerup->GetPosition().y + (powerup->GetScale().y / 2.0f) &&
					(powerup->GetTypeID() == 1001 || powerup->GetTypeID() == 1002 || 
						powerup->GetTypeID() == 1003 || powerup->GetTypeID() == 1004 || powerup->GetTypeID() == 1005))
				{
					m_SoundEngine->play2D("Audio/powerupGet.mp3", false);

					m_Score += 50;

					powerup->SetDelete(true);

					switch (powerup->GetTypeID())
					{
					case 1001:
						m_Lives++;
						break;

					case 1002:
						m_PlayerLaserLevel++;

						if (m_PlayerLaserLevel > m_PlayerLaserMax)
						{
							m_PlayerLaserLevel = m_PlayerLaserMax;
						}
						break;

					case 1003:
						if (m_PlayerSpeedLevel < m_PlayerSpeedMax)
						{
							player->SetMovementSpeed(player->GetMovementSpeed() + 150.0f);
							player->SetShootSpeed(player->GetShootSpeed() - 0.02);

							m_PlayerSpeedLevel++;
						}
						break;

					case 1004:
						if (m_PlayerShieldLevel < m_PlayerShieldMax)
						{
							m_PlayerShieldLevel++;
						}
						break;

					case 1005:
						m_ClearingScreen = true;
						m_ClearingScreenTimer = m_ClearingScreenTime;

						m_SoundEngine->play2D("Audio/blastWave.mp3", false);

						m_Objects.push_back(std::unique_ptr<Object>(new Object(m_Mesh, powerup->GetPosition(), glm::vec2(100.0f, 100.0f), m_ModelLocation,
							m_ShaderID, m_Width, m_Height, 0.0f, nullptr, "Textures/blastwave.png", 4005, true)));
						break;
					}
				}
			}

			// Screen clear powerup
			if (m_ClearingScreen)
			{
				m_ClearingScreenTimer -= deltaTime;

				for (auto& object : m_Objects)
				{
					switch (object->GetTypeID())
					{
					case 1:
					case 6:
					case 7:
					case 8:
						for (auto& blastWave : m_Objects)
						{
							if (blastWave->GetTypeID() == 4005)
							{
								if (object->GetPosition().x > blastWave->GetPosition().x - (blastWave->GetScale().x / 2.0f) &&
									object->GetPosition().x < blastWave->GetPosition().x + (blastWave->GetScale().x / 2.0f) &&
									object->GetPosition().y > blastWave->GetPosition().y - (blastWave->GetScale().y / 2.0f) &&
									object->GetPosition().y < blastWave->GetPosition().y + (blastWave->GetScale().y / 2.0f) &&
									!object->GetIsDead())
								{
									switch (object->GetTypeID())
									{
									case 1:
										m_Score += 20;
										break;
									case 6:
										m_Score += 80;
										break;
									case 7:
										m_Score += 70;
										break;
									case 8:
										m_Score += 40;
										break;
									}

									object->SetIsDead(true);
								}
							}
						}

						break;

					case 5001:
					case 5002:
					case 5003:
						for (auto& blastWave : m_Objects)
						{
							if (blastWave->GetTypeID() == 4005)
							{
								if (object->GetPosition().x > blastWave->GetPosition().x - (blastWave->GetScale().x / 2.0f) &&
									object->GetPosition().x < blastWave->GetPosition().x + (blastWave->GetScale().x / 2.0f) &&
									object->GetPosition().y > blastWave->GetPosition().y - (blastWave->GetScale().y / 2.0f) &&
									object->GetPosition().y < blastWave->GetPosition().y + (blastWave->GetScale().y / 2.0f))
								{
									object->SetDelete(true);
								}
							}
						}

						break;

					case 4005:
						object->SetScale(object->GetScale() + 1000.0f * deltaTime);
					}
				}

				if (m_ClearingScreenTimer < 0.0f)
				{
					m_ClearingScreen = false;


					for (auto& blastWave : m_Objects)
					{
						if (blastWave->GetTypeID() == 4005)
						{
							blastWave->SetDelete(true);
						}
					}
				}
			}


			// If player has 0 lives
			if (m_Lives <= 0 && !player->GetIsDead())
			{
				// Game over specifics
				m_SoundEngine->play2D("Audio/GameOver.wav");

				// Find game over screen and set to render it
				for (auto& gameOverScreen : m_Objects)
				{
					if (gameOverScreen->GetTypeID() == 20)
					{
						gameOverScreen->SetRender(true);
					}
				}

				// Set game over flag to true
				m_GameOver = true;

				m_GameOverTimer = m_GameOverTime;

				EndGame();
			}

			// If player has too many lives, limit it to max of 4
			if (m_Lives > m_MaxLives)
			{
				m_Lives = m_MaxLives;
			}

			// Shoot player lazers
			CountPlayerLazers();

			unsigned int lazerLimit = (m_PlayerLaserLevel + 1);

			if (player->GetShootLazer() && m_NumOfPlayerLazers < lazerLimit)
			{
				m_SoundEngine->play2D("Audio/lazer2.wav", false);

				switch (m_PlayerLaserLevel)
				{
				case 0:
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, player->GetPosition() + glm::vec2(2.0f, player->GetScale().y / 2.0f),
						player->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, player->GetMovementSpeed() * 2.0f, 5001)));
					player->SetShootLazer(false);
					break;

				case 1:
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, player->GetPosition() + glm::vec2(2.0f + player->GetScale().x / 4.0f, player->GetScale().y / 4.0f),
						player->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, player->GetMovementSpeed() * 2.0f, 5001)));
					player->SetShootLazer(false);

					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, player->GetPosition() + glm::vec2(2.0f - player->GetScale().x / 4.0f, player->GetScale().y / 4.0f),
						player->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, player->GetMovementSpeed() * 2.0f, 5001)));
					player->SetShootLazer(false);
					break;

				case 2:
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, player->GetPosition() + glm::vec2(2.0f + player->GetScale().x / 4.0f, player->GetScale().y / 4.0f),
						player->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, player->GetMovementSpeed() * 2.0f, 5001)));
					player->SetShootLazer(false);

					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, player->GetPosition() + glm::vec2(2.0f, player->GetScale().y / 2.0f),
						player->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, player->GetMovementSpeed() * 2.0f, 5001)));
					player->SetShootLazer(false);

					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, player->GetPosition() + glm::vec2(2.0f - player->GetScale().x / 4.0f, player->GetScale().y / 4.0f),
						player->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, player->GetMovementSpeed() * 2.0f, 5001)));
					player->SetShootLazer(false);
					break;
				}
			}

			// Check if player is being hit by enemy lasers
			for (auto& lazer : m_Objects)
			{
				if (lazer->GetTypeID() == 5002 || lazer->GetTypeID() == 5003)
				{
					if ((lazer->GetPosition().x > player->GetPosition().x - (player->GetScale().x / 3.7f) &&
						lazer->GetPosition().x < player->GetPosition().x + (player->GetScale().x / 3.7f) &&
						lazer->GetPosition().y > player->GetPosition().y - (player->GetScale().y / 3.7f) &&
						lazer->GetPosition().y < player->GetPosition().y + (player->GetScale().y / 3.7f)) &&
						!player->GetInvincible() && !player->GetIsDead())
					{
						m_SoundEngine->play2D("Audio/explosion2.wav", false);

						lazer->SetDelete(true);

						UpdateShield(*player);
					}
				}
			}

			if (m_Level == m_LastLevel + 1 && m_LevelTimer <= 0.0f)
			{
				ResetPowerups(true);

				player->SetDelete(true);
			}

			// Lives display

			// Now we are going to render the same object multiple times to show how many lives the player has.

			// Check if lives is greater than 0
			if (m_Lives > 1)
			{
				// We are going to use our main shader (because we probably were just rendering text
				m_MainShader->UseShader();
				// We are going to change the position based on an iterating integer called i
				glm::vec2 newPos;
				// Find miniship
				for (auto& miniShip : m_Objects)
				{
					if (miniShip->GetTypeID() == 21)
					{
						// Set position to original position

						miniShip->SetPosition(m_OriginalLifePos);

						// Loop lives - 1 times
						for (int i = 0; i < m_Lives - 1; i++)
						{
							// Set new position based on original position
							newPos = glm::vec2(m_OriginalLifePos.x - ((m_Width / 5.0f) * i), miniShip->GetPosition().y);
							miniShip->SetPosition(newPos);
							// Render again
							miniShip->SetRender(true);
							miniShip->Render();
							miniShip->SetRender(false);
						}
					}
				}
			}

			// Shield position is player's
			for (auto& shield : m_Objects)
			{
				switch (shield->GetTypeID())
				{
				case 3001:
				case 3002:
				case 3003:
					shield->SetPosition(player->GetPosition());
				}
			}
		}
	}

	// Spawn a star every starInterval seconds
	m_StarTimer -= deltaTime;

	if (m_StarTimer <= 0.0f)
	{
		m_StarTimer = m_StarInterval;

		m_Objects.insert(m_Objects.begin(), std::unique_ptr<Star>(new Star(m_Mesh, glm::vec2(0.0f, 0.0f), glm::vec2(50.0f, 50.0f), m_ModelLocation,
			m_ShaderID, m_Width, m_Height, 30.0f, false)));
	}

	// Add laser to objects if enemy is shooting
	for (auto& starship : m_Objects)
	{
		switch (starship->GetTypeID())
		{
		case 1:
		case 6:
		case 7:
		case 8:
			if (starship->GetShootLazer())
			{
				m_SoundEngine->play2D("Audio/enemylazer.wav", false);

				switch (starship->GetTypeID())
				{
				case 1:
				case 7:
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, starship->GetPosition() + glm::vec2(2.0f, -starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f, 5002)));
					break;

				case 6:
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, starship->GetPosition() + glm::vec2(2.0f - starship->GetScale().x / 4.0f, -starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f, 5002)));
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, starship->GetPosition() + glm::vec2(2.0f + starship->GetScale().x / 4.0f, -starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f, 5002)));
					break;

				case 8:
					// Top right corner
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh, 
						starship->GetPosition() + glm::vec2(starship->GetScale().x / 4.0f, starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f, 
						5003, glm::normalize(glm::vec2(1.0f, 1.0f)))));

					// Top left corner
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh,
						starship->GetPosition() + glm::vec2(-starship->GetScale().x / 4.0f, starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f,
						5003, glm::normalize(glm::vec2(-1.0f, 1.0f)))));

					// Bottom right corner
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh,
						starship->GetPosition() + glm::vec2(starship->GetScale().x / 4.0f, -starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f,
						5003, glm::normalize(glm::vec2(1.0f, -1.0f)))));

					// Bottom left corner
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh,
						starship->GetPosition() + glm::vec2(-starship->GetScale().x / 4.0f, -starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f,
						5003, glm::normalize(glm::vec2(-1.0f, -1.0f)))));

					// Top
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh,
						starship->GetPosition() + glm::vec2(0.0f, starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f,
						5003, glm::normalize(glm::vec2(0.0f, 1.0f)))));

					// Bottom
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh,
						starship->GetPosition() + glm::vec2(0.0f, -starship->GetScale().y / 4.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f,
						5003, glm::normalize(glm::vec2(0.0f, -1.0f)))));

					// Left
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh,
						starship->GetPosition() + glm::vec2(-starship->GetScale().x / 4.0f, 0.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f,
						5003, glm::normalize(glm::vec2(-1.0f, 0.0f)))));

					// Right
					m_Objects.push_back(std::unique_ptr<Lazer>(new Lazer(m_Mesh,
						starship->GetPosition() + glm::vec2(starship->GetScale().x / 4.0f, 0.0f),
						starship->GetScale(), m_ModelLocation, m_ShaderID, m_Width, m_Height, starship->GetMovementSpeed() * 2.6f,
						5003, glm::normalize(glm::vec2(1.0f, 0.0f)))));
					break;
				}

				starship->SetShootLazer(false);
			}
		}
	}

	// Check if enemies are being hit by player lasers
	for (auto& laser : m_Objects)
	{
		if (laser->GetTypeID() == 5001)
		{
			for (auto& enemy : m_Objects)
			{
				if ((enemy->GetTypeID() == 1 || enemy->GetTypeID() == 6 || enemy->GetTypeID() == 7 || enemy->GetTypeID() == 8))
				{
					if (laser->GetPosition().x > enemy->GetPosition().x - (enemy->GetScale().x / 2.35f) &&
						laser->GetPosition().x < enemy->GetPosition().x + (enemy->GetScale().x / 2.35f) &&
						laser->GetPosition().y > enemy->GetPosition().y - (enemy->GetScale().y / 2.35f) &&
						laser->GetPosition().y < enemy->GetPosition().y + (enemy->GetScale().y / 2.35f) &&
						enemy->GetIsDead() == false)
					{
						switch (enemy->GetTypeID())
						{
						case 1:
						case 6:
						case 7:
							m_SoundEngine->play2D("Audio/explosion2.wav", false);
							break;
						case 8:
							if (enemy->GetHealth() == 1)
								m_SoundEngine->play2D("Audio/Enemy5Destroyed.mp3", false);
							else
								m_SoundEngine->play2D("Audio/explosion2.wav", false);
							break;
						}

						switch (enemy->GetTypeID())
						{
						case 1:
							m_Score += 20;
							break;
						case 6:
							m_Score += 80;
							break;
						case 7:
							m_Score += 70;
							break;
						case 8:
							m_Score += 40;
							break;
						}

						laser->SetDelete(true);
						enemy->SetIsDead(true);
					}
				}
			}
		}
	}

	/* POWERUPS */
	// Powerup spawns
	if (!m_GameOver && !m_TitleScreen && !m_YouWon && m_Powerups)
	{
		m_PowerUpSpawnTimer -= deltaTime;

		bool powerupSpawned= false;

		if (rand() % 10000000000 == 1 && m_PowerUpSpawnTimer <= 0.0f)
		{
			m_Objects.push_back(std::unique_ptr<Object>(new Powerup(m_Mesh, glm::vec2(100.0f, 100.0f),
				m_ModelLocation, m_ShaderID, m_Width, m_Height, 40.0f, 1003)));

			powerupSpawned = true;
		}

		if (rand() % 10000000000 == 2 && m_PowerUpSpawnTimer <= 0.0f)
		{
			m_Objects.push_back(std::unique_ptr<Object>(new Powerup(m_Mesh, glm::vec2(100.0f, 100.0f),
				m_ModelLocation, m_ShaderID, m_Width, m_Height, 40.0f, 1004)));

			powerupSpawned = true;
		}

		if (rand() % 10000000000 == 3 && m_PowerUpSpawnTimer <= 0.0f)
		{
			m_Objects.push_back(std::unique_ptr<Object>(new Powerup(m_Mesh, glm::vec2(100.0f, 100.0f),
				m_ModelLocation, m_ShaderID, m_Width, m_Height, 40.0f, 1002)));

			powerupSpawned = true;
		}

		if (rand() % 10000000000 == 5 && m_PowerUpSpawnTimer <= 0.0f)
		{
			m_Objects.push_back(std::unique_ptr<Object>(new Powerup(m_Mesh, glm::vec2(100.0f, 100.0f),
				m_ModelLocation, m_ShaderID, m_Width, m_Height, 40.0f, 1005)));

			powerupSpawned = true;
		}

		if (rand() % 10000000000 == 4 && m_PowerUpSpawnTimer <= 0.0f && !m_1UpSpawned)
		{
			m_Objects.push_back(std::unique_ptr<Object>(new Powerup(m_Mesh, glm::vec2(100.0f, 100.0f),
				m_ModelLocation, m_ShaderID, m_Width, m_Height, 40.0f, 1001)));

			powerupSpawned = true;
			m_1UpSpawned = true;
		}

		if (powerupSpawned)
		{
			m_PowerUpSpawnTimer = m_PowerUpSpawnTime;

			m_SoundEngine->play2D("Audio/powerupSpawn.wav", false);
		}
	}

	// Shield rendering
	for (auto& shield : m_Objects)
	{
		if (shield->GetTypeID() == 3001)
		{
			if (m_PlayerShieldLevel > 0 && !m_ShieldDestroyed && !m_TitleScreen && !m_GameOver && !m_YouWon && !m_ShieldDamageAnimation)
			{
				shield->SetRender(true);
			}
			else if (m_TitleScreen || m_GameOver || m_YouWon && !m_ShieldDamageAnimation)
			{
				shield->SetRender(false);
			}
		}
		if (shield->GetTypeID() == 3003 && m_ShieldDamageAnimation)
		{
			shield->SetRender(true);

			m_ShieldDamageTimer -= deltaTime;

			if (m_ShieldDamageTimer < 0.0f)
			{
				m_ShieldDamageAnimation = false;
			}
		}
		if (shield->GetTypeID() == 3003 && !m_ShieldDamageAnimation)
		{
			shield->SetRender(false);
		}
	}

	if (m_ShieldDestroyed && m_ShieldDeathTimer > 0.0f && !m_TitleScreen && !m_GameOver && !m_YouWon)
	{
		m_ShieldDeathTimer -= deltaTime;
		m_ShieldSwitchTimer -= deltaTime;

		if (m_ShieldSwitchTimer < 0.0f)
		{
			m_ShieldSwitchTimer = m_ShieldSwitchSpeed;
			m_ShieldSwitchToggle = !m_ShieldSwitchToggle;
		}

		for (auto& brokenShield : m_Objects)
		{
			if (m_ShieldSwitchToggle)
			{
				if (brokenShield->GetTypeID() == 3001)
				{
					brokenShield->SetRender(true);
				}
				if (brokenShield->GetTypeID() == 3002)
				{
					brokenShield->SetRender(false);
				}
			}
			else
			{
				if (brokenShield->GetTypeID() == 3001)
				{
					brokenShield->SetRender(false);
				}
				if (brokenShield->GetTypeID() == 3002)
				{
					brokenShield->SetRender(true);
				}
			}
		}
	}

	if (m_ShieldDestroyed && m_ShieldDeathTimer < 0.0f)
	{
		m_ShieldDestroyed = false;

		for (auto& brokenShield : m_Objects)
		{
			if (brokenShield->GetTypeID() == 3002)
			{
				brokenShield->SetRender(false);
			}
		}

		for (auto& brokenShield : m_Objects)
		{
			if (brokenShield->GetTypeID() == 3001)
			{
				brokenShield->SetRender(false);
			}
		}
	}

	// Wait until player presses key to exit game over screen
	if (m_GameOver)
	{
		PressedButtonPrompt(deltaTime);
	}
}


void Game::IncrLevel()
{
	m_Level++;

	m_ShownLevelCounter = false;

	m_PromptFlashTimer = m_PromptFlashTime * 4.0f;
	m_1UpSpawned = false;

	if (m_Level > 1)
	{
		m_SoundEngine->play2D("Audio/nextLevel2.wav", false);
		m_Score += 500;
	}

	switch (m_Level)
	{
	case 1:
		m_EnemySpawnTime = 3.0f;
		m_LevelTimer = 30.0f;

		for (auto& titleScreen : m_Objects)
		{
			if (titleScreen->GetTypeID() == 15 || titleScreen->GetTypeID() == 16)
			{
				titleScreen->SetRender(false);
			}
		}
		break;

	case 2:

		m_EnemySpawnTime = 2.6f;
		m_LevelTimer = 35.0f;

		break;

	case 3:
		m_EnemySpawnTime = 3.2f;
		m_LevelTimer = 40.0f;

		break;

	case 4:

		m_EnemySpawnTime = 3.1f;

		m_LevelTimer = 45.0f;

		break;

	case 5:

		m_EnemySpawnTime = 3.0f;
		m_LevelTimer = 50.0f;

		break;

	case 6:

		m_EnemySpawnTime = 2.8;
		m_LevelTimer = 55.0f;

		break;

	case 7:

		m_EnemySpawnTime = 4.0f;
		m_LevelTimer = 60.0f;

		break;

	case 8:

		m_EnemySpawnTime = 6.4f;
		m_LevelTimer = 55.0f;

		break;

	case 9:
		m_EnemySpawnTime = 4.0f;
		m_LevelTimer = 40.0f;

		break;

	case 10:
		m_EnemySpawnTime = 6.5f;
		m_LevelTimer = 60.0f;

		break;
	}


	if (m_Level != 1)
	{
		m_Lives++;
	}

	m_EnemySpawnTimer = m_EnemySpawnTime;
}

void Game::UpdateGameLevel(GLfloat& deltaTime)
{
	struct EnemySettings
	{
		float movementSpeed = 0.0f;
		float shootingSpeed = 0.0f;
		float turnSpeed = 0.0f;

		unsigned int number = 0;
	};

	struct Case
	{
		EnemySettings enemy1;
		EnemySettings enemy2;
		EnemySettings enemy3;
		EnemySettings enemy4;
	};

	Case case1;
	Case case2;

	// Level 1 enemy spawns
	if (m_Level > 0 && m_ShownLevelCounter)
	{
		m_EnemySpawnTimer -= deltaTime;
		m_LevelTimer -= deltaTime;

		switch (m_Level)
		{
		case 1:
			case1.enemy1.movementSpeed = rand() % 2 == 1 ? 100.0f : 150.0f;
			case1.enemy1.shootingSpeed = rand() % 2 == 1 ? 0.9f : 0.7f;
			case1.enemy1.turnSpeed = rand() % 2 == 1 ? 0.1f : 0.3f;
					   
			case1.enemy1.number = 1;

			case2 = case1;
			break;

		case 2:
			case1.enemy1.movementSpeed = rand() % 2 == 1 ? 160.0f : 175.0f;
			case1.enemy1.shootingSpeed = rand() % 2 == 1 ? 0.6f : 0.55f;
			case1.enemy1.turnSpeed = rand() % 2 == 1 ? 0.4f : 0.6f;

			case1.enemy1.number = 1;

			case2 = case1;

			case2.enemy1.number = 2;
			break;

		case 3:
			case1.enemy2.movementSpeed = rand() % 2 == 1 ? 189.0f : 200.0f;
			case1.enemy2.shootingSpeed = rand() % 2 == 1 ? 0.48 : 0.53f;
			case1.enemy2.turnSpeed = rand() % 2 == 1 ? 0.5f : 0.6f;
			
			case1.enemy2.number = 1;

			case2.enemy1.movementSpeed = rand() % 2 == 1 ? 172.0f : 181.0f;
			case2.enemy1.shootingSpeed = rand() % 2 == 1 ? 0.43f : 0.38f;
			case2.enemy1.turnSpeed = rand() % 2 == 1 ? 0.7f : 0.75f;
			
			case2.enemy2.movementSpeed = rand() % 2 == 1 ? 189.0f : 200.0f;
			case2.enemy2.shootingSpeed = rand() % 2 == 1 ? 0.48 : 0.53f;
			case2.enemy2.turnSpeed = rand() % 2 == 1 ? 0.5f : 0.6f;

			case2.enemy1.number = 1;
			case2.enemy2.number = 1;
			break;
				
		case 4:
			case1.enemy2.movementSpeed = rand() % 2 == 1 ? 215.0f : 230.0f;
			case1.enemy2.shootingSpeed = rand() % 2 == 1 ? 0.35f : 0.39f;
			case1.enemy2.turnSpeed = rand() % 2 == 1 ? 0.9f : 1.4f;

			case1.enemy2.number = 1;

			case2.enemy2.movementSpeed = rand() % 2 == 1 ? 205.0f : 210.0f;
			case2.enemy2.shootingSpeed = rand() % 2 == 1 ? 0.48f : 0.54f;
			case2.enemy2.turnSpeed = rand() % 2 == 1 ? 0.5f : 1.3f;

			case2.enemy2.number = 2;
			break;

		case 5:
			case1.enemy2.movementSpeed = rand() % 2 == 1 ? 215.0f : 230.0f;
			case1.enemy2.shootingSpeed = rand() % 2 == 1 ? 0.41f : 0.47f;
			case1.enemy2.turnSpeed = rand() % 2 == 1 ? 0.5f : 0.7f;

			case1.enemy2.number = 2;

			case2.enemy1.movementSpeed = rand() % 2 == 1 ? 200.0f : 190.0f;
			case2.enemy1.shootingSpeed = rand() % 2 == 1 ? 0.37f : 0.39f;
			case2.enemy1.turnSpeed = rand() % 2 == 1 ? 1.1f : 1.3f;

			case2.enemy1.number = 2;
			break;

		case 6:
			case1.enemy3.movementSpeed = rand() % 2 == 1 ? 154.0f : 167.0f;
			case1.enemy3.shootingSpeed = rand() % 2 == 1 ? 0.7f : 0.75f;
			case1.enemy3.turnSpeed = rand() % 2 == 1 ? 0.1f : 0.2;

			case1.enemy3.number = 1;

			case2 = case1;
			break;

		case 7:
			case1.enemy3.movementSpeed = rand() % 2 == 1 ? 154.0f : 167.0f;
			case1.enemy3.shootingSpeed = rand() % 2 == 1 ? 0.65f : 0.60f;
			case1.enemy3.turnSpeed = rand() % 2 == 1 ? 0.1f : 0.2;

			case1.enemy3.number = 1;

			case2.enemy3.movementSpeed = rand() % 2 == 1 ? 134.0f : 125.0f;
			case2.enemy3.shootingSpeed = rand() % 2 == 1 ? 0.7f : 0.75f;
			case2.enemy3.turnSpeed = rand() % 2 == 1 ? 0.1f : 0.2;

			case2.enemy3.number = 2;
			break;

		case 8:
			case1.enemy2.movementSpeed = rand() % 2 == 1 ? 240.0f : 250.0f;
			case1.enemy2.shootingSpeed = rand() % 2 == 1 ? 0.29f : 0.37f;
			case1.enemy2.turnSpeed = rand() % 2 == 1 ? 0.6f : 0.8f;

			case1.enemy2.number = 1;

			case1.enemy3.movementSpeed = rand() % 2 == 1 ? 154.0f : 167.0f;
			case1.enemy3.shootingSpeed = rand() % 2 == 1 ? 0.65f : 0.60f;
			case1.enemy3.turnSpeed = rand() % 2 == 1 ? 0.1f : 0.2;

			case1.enemy3.number = 1;

			case2.enemy1.movementSpeed = rand() % 2 == 1 ? 200.0f : 190.0f;
			case2.enemy1.shootingSpeed = rand() % 2 == 1 ? 0.37f : 0.39f;
			case2.enemy1.turnSpeed = rand() % 2 == 1 ? 1.1f : 1.3f;

			case2.enemy1.number = 1;

			case2.enemy3.movementSpeed = rand() % 2 == 1 ? 134.0f : 125.0f;
			case2.enemy3.shootingSpeed = rand() % 2 == 1 ? 0.7f : 0.75f;
			case2.enemy3.turnSpeed = rand() % 2 == 1 ? 0.1f : 0.2;

			case2.enemy3.number = 1;
			break;

		case 9:
			case1.enemy4.movementSpeed = rand() % 2 == 1 ? 100.0f : 150.0f;
			case1.enemy4.shootingSpeed = rand() % 2 == 1 ? 1.1f : 1.2f;

			case1.enemy4.number = 1;

			case2 = case1;
			break;

		case 10:
			case1.enemy4.movementSpeed = rand() % 2 == 1 ? 140.0f : 170.0f;
			case1.enemy4.shootingSpeed = rand() % 2 == 1 ? 1.1f : 1.2f;

			case1.enemy4.number = 1;

			case1.enemy3.movementSpeed = rand() % 2 == 1 ? 154.0f : 167.0f;
			case1.enemy3.shootingSpeed = rand() % 2 == 1 ? 0.65f : 0.60f;
			case1.enemy3.turnSpeed = rand() % 2 == 1 ? 0.1f : 0.2;

			case1.enemy3.number = 1;

			case2.enemy4.movementSpeed = rand() % 2 == 1 ? 140.0f : 170.0f;
			case2.enemy4.shootingSpeed = rand() % 2 == 1 ? 1.1f : 1.2f;
				
			case2.enemy4.number = 1;

			case2.enemy2.movementSpeed = rand() % 2 == 1 ? 215.0f : 230.0f;
			case2.enemy2.shootingSpeed = rand() % 2 == 1 ? 0.35f : 0.39f;
			case2.enemy2.turnSpeed = rand() % 2 == 1 ? 0.9f : 1.4f;
				
			case2.enemy2.number = 1;
			
			break;
		}

		if (m_EnemySpawnTimer <= 0.0f)
		{
			m_EnemySpawnTimer = m_EnemySpawnTime;

			if (rand() % 3 == 1 || rand() % 3 == 2)
			{
				for (int i = 0; i < case1.enemy1.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 2, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 2), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case1.enemy1.movementSpeed,
						case1.enemy1.shootingSpeed,
						case1.enemy1.turnSpeed, 1, m_SoundEngine)));
				}

				for (int i = 0; i < case1.enemy2.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 2, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 2), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case1.enemy2.movementSpeed,
						case1.enemy2.shootingSpeed,
						case1.enemy2.turnSpeed, 6, m_SoundEngine)));
				}

				for (int i = 0; i < case1.enemy3.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 7, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 3), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case1.enemy3.movementSpeed,
						case1.enemy3.shootingSpeed,
						case1.enemy3.turnSpeed, 7, m_SoundEngine)));
				}
				for (int i = 0; i < case1.enemy4.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 2, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 2), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case1.enemy4.movementSpeed,
						case1.enemy4.shootingSpeed,
						case1.enemy4.turnSpeed, 8, m_SoundEngine)));
				}
			}
			else
			{
				for (int i = 0; i < case2.enemy1.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 2, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 2), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case2.enemy1.movementSpeed,
						case2.enemy1.shootingSpeed,
						case2.enemy1.turnSpeed, 1, m_SoundEngine)));
				}

				for (int i = 0; i < case2.enemy2.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 2, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 2), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case2.enemy2.movementSpeed,
						case2.enemy2.shootingSpeed,
						case2.enemy2.turnSpeed, 6, m_SoundEngine)));
				}

				for (int i = 0; i < case2.enemy3.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 7, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 3), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case2.enemy3.movementSpeed,
						case2.enemy3.shootingSpeed,
						case2.enemy3.turnSpeed, 7, m_SoundEngine)));
				}
				for (int i = 0; i < case2.enemy4.number; i++)
				{
					m_Objects.push_back(std::unique_ptr<EnemyStarship>(new EnemyStarship(m_Mesh,
						rand() % 2 == 1 ? glm::vec2(rand() % (int)m_Width / 2, m_Height / 2) : glm::vec2(-(rand() % (int)m_Width / 2), m_Height / 2.0f),
						glm::vec2(100.0f, 100.0f), m_ModelLocation, m_ShaderID, m_Width, m_Height,
						case2.enemy4.movementSpeed,
						case2.enemy4.shootingSpeed,
						case2.enemy4.turnSpeed, 8, m_SoundEngine)));
				}
			}

			if (rand() % 2 == 1)
				m_EnemySpawnTime *= 1.01;
			else
				m_EnemySpawnTime *= 0.97;
		}

		if (m_LevelTimer <= 0.0f && m_Level < m_LastLevel + 1)
			IncrLevel();
	}
}

void Game::DeleteEntities()
{
	for (auto& nonStarEntities : m_Objects)
	{
		switch (nonStarEntities->GetTypeID())
		{
		case 0:
		case 1:
		case 6:
		case 7:
		case 8:
		case 5001:
		case 5002:
		case 5003:
		case 1001:
		case 1002:
		case 1003:
		case 1004:
		case 1005:
		case 3001:
		case 3002:
		case 4005:
			nonStarEntities->SetDelete(true);
		}
	}
}
