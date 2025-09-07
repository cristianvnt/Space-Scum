#include "Game.h"

Game::Game(const std::string& filePath) : _gameManager{ filePath }
{
}

GameManagerSettings Game::GetSettings()
{
	return _gameManager.GetSettings();
}

void Game::Run()
{
	SetTargetFPS(_gameManager.GetSettings().fps);
	SetExitKey(KEY_NULL);

	while (!WindowShouldClose())
	{
		if (!_gameManager.Update(GetFrameTime()))
			break;

		_gameManager.Draw();
	}

	_gameManager.Close();
}
