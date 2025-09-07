#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H
#include "GameState.h"

struct PauseSettings
{
	int width{};
	int height{};
};

class PauseState : public GameState
{
public:
	PauseState(const PauseSettings& ps);

	StateType Update(float dt) override;
	void Draw() override;
	StateType ProcessInput(float dt) override;
	StateType GetStateType() const override { return StateType::PAUSE; }
};

#endif
