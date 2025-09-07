#ifndef CONFIG_H
#define CONFIG_H

namespace GAME
{
	namespace Settings
	{
		constexpr const int SCREEN_WIDTH = 1000;
		constexpr const int SCREEN_HEIGHT = 700;
		constexpr const float MARGIN = 70.f;
		constexpr const int FPS = 60;
	}

	namespace Gameplay
	{
		constexpr const float SPAWN_TIME = 0.7f;
		constexpr const float COLLISION_TIME = 0.5f;
		constexpr const float PLAYER_SPEED = 500.f;
		constexpr const float ENEMY_SPEED = 80.f;
		constexpr const float BULLET_SPEED = 1000.f;
	}
}

#endif
