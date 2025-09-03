#ifndef GAME_H
#define GAME_H

#include "GameMgr.h"

class Game
{
private:
	GameMgr _gameMgr;

public:
	Game() : _gameMgr{ InitSettings() } {}

	GameMgrSettings InitSettings();
	void Run();
};

#endif
