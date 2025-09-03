#include "MainMenuState.h"
#include "raylib.h"

MainMenuState::MainMenuState(const MainMenuSettings& mms)
{
}

void MainMenuState::Update(float dt)
{
	// nothing idk
}

void MainMenuState::Draw()
{
	DrawText("Press ENTER to START GAME", 100, 300, 50, BLACK);
}

StateType MainMenuState::ProcessInput(float dt)
{
	if (IsKeyPressed(KEY_ENTER))
		return StateType::GAMEPLAY;
	if (IsKeyPressed(KEY_ESCAPE))
		return StateType::QUIT;

	return StateType::NONE;
}
