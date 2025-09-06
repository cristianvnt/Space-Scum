#ifndef GAME_STATE_H
#define GAME_STATE_H

enum class StateType
{
	NONE,
	MAIN_MENU,
	GAMEPLAY,
	PAUSE,
	GAME_OVER,
	QUIT,
	POP
};

class GameState
{
public:
	virtual ~GameState() = default;
	virtual StateType Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual StateType ProcessInput(float dt) = 0;
	virtual StateType GetStateType() const = 0;
};

#endif
