#include "Core/Game.h"

int main()
{
	Game game{ "config/Settings.ini" };
	InitWindow(game.GetSettings().width, game.GetSettings().height, "BLABLEBLU");
	game.Run();
}

