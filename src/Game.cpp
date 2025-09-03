#include "Game.h"
#include "Config.h"

using namespace GAME;

GameMgrSettings Game::InitSettings()
{
	GameMgrSettings temp;
	temp.width = Settings::SCREEN_WIDTH;
	temp.height = Settings::SCREEN_HEIGHT;
	temp.fps = Settings::FPS;
	temp.margin = Settings::MARGIN;

	return temp;
}

GameMgrSettings Game::GetSettings()
{
	return _gameMgr.GetSettings();
}

void Game::Run()
{
	SetTargetFPS(_gameMgr.GetSettings().fps);
	SetExitKey(KEY_Q);
	bool shouldClose = false;

	while (!WindowShouldClose() && !shouldClose)
	{
		StateType currState = _gameMgr.ProcessInput(GetFrameTime());
		StateType activeState = _gameMgr.GetCurrentState();

		switch (currState)
		{
		case StateType::MAIN_MENU:
		case StateType::GAMEPLAY:
		case StateType::GAME_OVER:
			if (activeState != currState)
			{
				_gameMgr.PopState();
				_gameMgr.PushState(currState);
			}
			break;
		case StateType::PAUSE:
			if (activeState != StateType::PAUSE)
				_gameMgr.PushState(currState);
			break;
		case StateType::POP:
			_gameMgr.PopState();
			break;
		case StateType::NONE:
			break;
		case StateType::QUIT:
			shouldClose = true;
			break;
		default:
			break;
		}
		_gameMgr.Update(GetFrameTime());
		_gameMgr.Draw();
	}

	_gameMgr.Close();
}
