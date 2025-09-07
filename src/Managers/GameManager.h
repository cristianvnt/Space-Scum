#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "raylib.h"
#include "States/GameState.h"
#include "States/MainMenuState.h"
#include "States/GameplayState.h"
#include "States/PauseState.h"
#include "States/GameOverState.h"

#include <stack>

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
	GameManagerSettings _gameMgrSettings;

	PlayerSettings InitPlayerSettings();
public:
	GameManager(const GameManagerSettings& gs);
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
