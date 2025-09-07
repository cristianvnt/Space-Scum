#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stack>

#include "raylib.h"
#include "States/GameState.h"
#include "States/MainMenuState.h"
#include "States/GameplayState.h"
#include "States/PauseState.h"
#include "States/GameOverState.h"
#include "ConfigManager.h"

struct GameManagerSettings
{
	int width{};
	int height{};
	int fps{};
	float margin{};
};

class GameManager
{
private:
	std::stack<GameState*> _gameStates{};
	ConfigManager _configManager;
	GameManagerSettings _gameManagerSettings;

	PlayerSettings InitPlayerSettings();
	EnemySettings InitEnemySettings();
	GameManagerSettings InitSettings();
public:
	GameManager(const std::string& filePath);
	~GameManager();

	bool Update(float dt);
	void Draw();
	StateType GetCurrentState() const;

	const GameManagerSettings GetSettings() const;
	void PushState(const StateType& st);
	void PopState();
	void Close();
};

#endif
