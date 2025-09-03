#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H
#include "GameState.h"

struct GameOverSettings
{
	int width{};
	int height{};
};

class GameOverState : public GameState
{
public:
	GameOverState(const GameOverSettings& gos);

	void Update(float dt) override;
	void Draw() override;
	StateType ProcessInput(float dt) override;
	StateType GetStateType() const override { return StateType::GAME_OVER; }
};

#endif
