#include "Game.h"
#include <iostream>

int main()
{
	Game game;
	InitWindow(game.GetSettings().width, game.GetSettings().height, "BLABLEBLU");
	game.Run();
}

