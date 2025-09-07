#include "Game.h"
#include "utils/Config.h"

using namespace GAME;

GameManagerSettings Game::InitSettings()
{
	GameManagerSettings temp;
	temp.width = Settings::SCREEN_WIDTH;
	temp.height = Settings::SCREEN_HEIGHT;
	temp.fps = Settings::FPS;
	temp.margin = Settings::MARGIN;

	return temp;
}

GameManagerSettings Game::GetSettings()
{
	return _gameMgr.GetSettings();
}

void Game::Run()
{
	SetTargetFPS(_gameMgr.GetSettings().fps);
	SetExitKey(KEY_NULL);

	while (!WindowShouldClose())
	{
		if (!_gameMgr.Update(GetFrameTime()))
			break;

		_gameMgr.Draw();
	}

	_gameMgr.Close();
}
