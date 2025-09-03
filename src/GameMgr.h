#ifndef GAME_MGR_H
#define GAME_MGR_H
#include "raylib.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "PauseState.h"
#include "GameOverState.h"

#include <stack>

struct GameMgrSettings
{
	int width{};
	int height{};
	int fps{};
	float margin{};
};

class GameMgr
{
private:
	std::stack<GameState*> _gameStates{};
	GameMgrSettings _gameMgrSettings;

	PlayerSettings InitPlayerSettings();
public:
	GameMgr(const GameMgrSettings& gs);
	~GameMgr();

	void Update(float dt);
	void Draw();
	StateType ProcessInput(float dt);
	StateType GetCurrentState() const;

	const GameMgrSettings GetSettings() const;
	void PushState(const StateType& st);
	void PopState();
	void Close();
};

#endif
