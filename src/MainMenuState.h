#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H
#include "GameState.h"

struct MainMenuSettings
{
	int width{};
	int height{};
};

class MainMenuState : public GameState
{
public:
	MainMenuState(const MainMenuSettings& mms);

	void Update(float dt) override;
	void Draw() override;
	StateType ProcessInput(float dt) override;
	StateType GetStateType() const override { return StateType::MAIN_MENU; }
};

#endif
