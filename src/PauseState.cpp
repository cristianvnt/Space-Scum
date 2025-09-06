#include "PauseState.h"
#include "raylib.h"

PauseState::PauseState(const PauseSettings& ps)
{

}

StateType PauseState::Update(float dt)
{
	return StateType::PAUSE;
}

void PauseState::Draw()
{
	DrawText("PAUSED", 350, 200, 60, BLACK);
	DrawText("Press P to RESUME", 200, 300, 50, BLACK);
}

StateType PauseState::ProcessInput(float dt)
{
	if (IsKeyPressed(KEY_P))
		return StateType::POP;

	return StateType::NONE;
}
