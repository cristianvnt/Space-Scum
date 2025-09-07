#ifndef GAME_H
#define GAME_H

#include "Managers/GameManager.h"

class Game
{
private:
	GameManager _gameManager;

	GameManagerSettings InitSettings();
public:
	Game(const std::string& filePath);

	GameManagerSettings GetSettings();
	void Run();
};

#endif
