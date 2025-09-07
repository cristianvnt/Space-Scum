#include "GameManager.h"

GameManager::GameManager(const std::string& filePath) : _configManager{ filePath }, _gameManagerSettings { InitSettings() }
{
	PushState(StateType::MAIN_MENU);
}

GameManagerSettings GameManager::InitSettings()
{
	GameManagerSettings temp{};
	temp.width = _configManager.GetValue<int>("GAME", "SCREEN_WIDTH");
	temp.height = _configManager.GetValue<int>("GAME", "SCREEN_HEIGHT");
	temp.margin = _configManager.GetValue<float>("GAME", "MARGIN");
	temp.fps = _configManager.GetValue<int>("GAME", "FPS");

	return temp;
}

PlayerSettings GameManager::InitPlayerSettings()
{
	PlayerSettings temp{};
	temp.body = { (float)_gameManagerSettings.width / 2.f, (float)_gameManagerSettings.height / 1.5f, 50.f, 70.f };
	temp.speed = _configManager.GetValue<float>("PLAYER", "PLAYER_SPEED");
	temp.velocity = { 0.f, 0.f };
	temp.color = { BLUE };
	temp.score = 0;
	temp.life = { (float)_gameManagerSettings.width - 50.f, 20.f, 30.f, 30.f };
	temp.lives = _configManager.GetValue<int>("PLAYER", "LIVES");
	temp.vulnerable = true;
	temp.collisionTimer = { };

	return temp;
}

EnemySettings GameManager::InitEnemySettings()
{
	EnemySettings temp{};
	temp.body = { 0.f, 0.f, 50.f, 30.f };
	temp.velocity = { 0.f, 0.f };
	temp.color = MAGENTA;
	temp.speed = _configManager.GetValue<float>("ENEMY", "ENEMY_SPEED");
	temp.active = true;
	temp.spawnTime = _configManager.GetValue<float>("ENEMY", "SPAWN_TIME");

	return temp;
}

GameManager::~GameManager()
{
	while (!_gameStates.empty())
	{
		delete _gameStates.top();
		_gameStates.pop();
	}
}

bool GameManager::Update(float dt)
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

void GameManager::Draw()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	if (!_gameStates.empty() && _gameStates.top())
		_gameStates.top()->Draw();

	EndDrawing();
}

StateType GameManager::GetCurrentState() const
{
	if (_gameStates.empty())
		return StateType::NONE;

	return _gameStates.top()->GetStateType();
}

const GameManagerSettings GameManager::GetSettings() const
{
	return _gameManagerSettings;
}

void GameManager::PushState(const StateType& st)
{
	switch (st)
	{
	case StateType::MAIN_MENU:
		_gameStates.emplace(new MainMenuState(MainMenuSettings{ _gameManagerSettings.width, _gameManagerSettings.height }));
		break;
	case StateType::GAMEPLAY:
		_gameStates.emplace(new GameplayState(GameplaySettings{ _gameManagerSettings.width, _gameManagerSettings.height, _gameManagerSettings.margin, 
			InitPlayerSettings(), InitEnemySettings() }));
		break;
	case StateType::PAUSE:
		_gameStates.emplace(new PauseState(PauseSettings{ _gameManagerSettings.width, _gameManagerSettings.height }));
		break;
	case StateType::GAME_OVER:
		_gameStates.emplace(new GameOverState(GameOverSettings{ _gameManagerSettings.width, _gameManagerSettings.height }));
		break;
	default:
		break;
	}
}

void GameManager::PopState()
{
	if (_gameStates.empty())
		return;

	delete _gameStates.top();
	_gameStates.pop();
}

void GameManager::Close()
{
	CloseWindow();
}
