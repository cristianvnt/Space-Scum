#include "Game.h"
#include <iostream>

int main()
{
	Game game;
	GameMgr settings = game.InitSettings();
	InitWindow(settings.GetSettings().width, settings.GetSettings().height, "BLABLEBLU");
	game.Run();
}

