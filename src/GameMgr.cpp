#include "GameMgr.h"

GameMgr::GameMgr(const GameMgrSettings& gs) : _gameMgrSettings{ gs }
{
	PushState(StateType::MAIN_MENU);
}

PlayerSettings GameMgr::InitPlayerSettings()
{
	PlayerSettings tmp{};
	tmp.body = { (float)_gameMgrSettings.width / 2.f, (float)_gameMgrSettings.height / 1.5f, 50.f, 70.f };
	tmp.speed = { 500.f };
	tmp.velocity = { 0.f, 0.f };
	tmp.color = { BLUE };
	tmp.score = { 0 };
	tmp.life = { (float)_gameMgrSettings.width - 50.f, 20.f, 30.f, 30.f };
	tmp.lives = { 5 };
	tmp.vulnerable = { true };
	tmp.collisionTimer = { };

	return tmp;
}

GameMgr::~GameMgr()
{
	while (!_gameStates.empty())
	{
		delete _gameStates.top();
		_gameStates.pop();
	}
}

bool GameMgr::Update(float dt)
{
	if (_gameStates.empty() || !_gameStates.top())
	{
		Close();
		return false;
	}

	StateType inputState = _gameStates.top()->ProcessInput(dt);

	switch (inputState)
	{
	case StateType::MAIN_MENU:
	case StateType::GAMEPLAY:
	case StateType::GAME_OVER:
		if (inputState != GetCurrentState())
		{
			PopState();
			PushState(inputState);
		}
		break;
	case StateType::PAUSE:
		if (StateType::PAUSE != GetCurrentState())
			PushState(inputState);
		break;
	case StateType::POP:
		PopState();
		if (_gameStates.empty())
			return false;
		break;
	case StateType::QUIT:
		PopState();
		return false;
		break;
	case StateType::NONE:
	default:
		_gameStates.top()->Update(dt);
		break;
	}

	return true;
}

void GameMgr::Draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	if (!_gameStates.empty() && _gameStates.top())
		_gameStates.top()->Draw();

	EndDrawing();
}

StateType GameMgr::GetCurrentState() const
{
	if (_gameStates.empty())
		return StateType::NONE;

	return _gameStates.top()->GetStateType();
}

const GameMgrSettings GameMgr::GetSettings() const
{
	return _gameMgrSettings;
}

void GameMgr::PushState(const StateType& st)
{
	switch (st)
	{
	case StateType::MAIN_MENU:
		_gameStates.emplace(new MainMenuState(MainMenuSettings{ _gameMgrSettings.width, _gameMgrSettings.height }));
		break;
	case StateType::GAMEPLAY:
		_gameStates.emplace(new GameplayState(GameplaySettings{ _gameMgrSettings.width, _gameMgrSettings.height, _gameMgrSettings.margin, InitPlayerSettings() }));
		break;
	case StateType::PAUSE:
		_gameStates.emplace(new PauseState(PauseSettings{ _gameMgrSettings.width, _gameMgrSettings.height }));
		break;
	case StateType::GAME_OVER:
		_gameStates.emplace(new GameOverState(GameOverSettings{ _gameMgrSettings.width, _gameMgrSettings.height }));
		break;
	default:
		break;
	}
}

void GameMgr::PopState()
{
	if (_gameStates.empty())
		return;

	delete _gameStates.top();
	_gameStates.pop();
}

void GameMgr::Close()
{
	CloseWindow();
}
