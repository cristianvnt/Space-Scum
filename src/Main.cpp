#include "Core/Game.h"

int main()
{
	Game game;
	InitWindow(game.GetSettings().width, game.GetSettings().height, "BLABLEBLU");
	game.Run();
}

