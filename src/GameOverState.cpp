#include "GameOverState.h"
#include "raylib.h"

GameOverState::GameOverState(const GameOverSettings& gos)
{

}

void GameOverState::Update(float dt)
{
	// nothing idk
}

void GameOverState::Draw()
{
	DrawText("GAME OVER", 300, 200, 60, BLACK);
	DrawText("Press R to RESTART", 200, 300, 50, BLACK);
}

StateType GameOverState::ProcessInput(float dt)
{
	if (IsKeyPressed(KEY_R))
		return StateType::GAMEPLAY;

	return StateType::NONE;
}
