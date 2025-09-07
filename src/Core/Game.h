#ifndef GAME_H
#define GAME_H

#include "Managers/GameManager.h"

class Game
{
private:
	GameManager _gameMgr;

	static GameManagerSettings InitSettings();
public:
	Game() : _gameMgr{ InitSettings() } {}

	GameManagerSettings GetSettings();
	void Run();
};

#endif
