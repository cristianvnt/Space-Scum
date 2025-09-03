#ifndef GAME_H
#define GAME_H

#include "GameMgr.h"

class Game
{
private:
	GameMgr _gameMgr;

	static GameMgrSettings InitSettings();
public:
	Game() : _gameMgr{ InitSettings() } {}

	GameMgrSettings GetSettings();
	void Run();
};

#endif
