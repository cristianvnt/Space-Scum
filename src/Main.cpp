#include "raylib.h"

#include <vector>
#include <iostream>
#include <random>

/*
*	Game defines
*/
constexpr const int SCREEN_WIDTH = 1000;
constexpr const int SCREEN_HEIGHT = 700;
constexpr const int FPS = 60;

/*
*	Function defines
*/
static void InitGame();
static void UpdateGame(float dt);
static void DrawStuff();
static void ProcessInput(float dt);
static void CheckBounds();

static void SpawnBullet();
static void UpdateBullets(float dt);

static void SpawnEnemies();
static void UpdateEnemies(float dt);
static Vector2 RandomPosition();
static void CheckCollisions();
static void CleanUp();

#pragma region TIMER
struct Timer
{
	float _lifeTime;
};

void StartTimer(Timer* timer, float lifeTime)
{
	if (!timer)
		return;
	
	timer->_lifeTime = lifeTime;
}

void UpdateTimer(Timer* timer)
{
	if (!timer || timer->_lifeTime < 0)
		return;

	timer->_lifeTime -= GetFrameTime();
}

bool TimerDone(Timer* timer)
{
	if (!timer)
		return false;

	return timer->_lifeTime <= 0;
}

#pragma endregion

struct Player
{
	Rectangle body{};
	float speed{};
};

struct Bullet
{
	Rectangle body{};
	float speed{};
	bool active{};
	Color color{};
};

struct Enemy
{
	Rectangle body{};
	float speed{};
	bool active{};
	Color color{};
};

Player player;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;

float enemySpawnTime = 0.5f;
Timer timer{};

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BLABLEBLU");

	InitGame();

	SetTargetFPS(FPS);
	SetExitKey(KEY_Q);

	StartTimer(&timer, enemySpawnTime);

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

		UpdateGame(dt);

		DrawStuff();
	}

	CloseWindow();
}

void InitGame()
{
	player.body = { (float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 1.5f, 50.f, 70.f };
	player.speed = 500;
}

void UpdateGame(float dt)
{
	UpdateTimer(&timer);

	CheckBounds();
	ProcessInput(dt);

	UpdateBullets(dt);
	UpdateEnemies(dt);
	CheckCollisions();
	CleanUp();

	if (TimerDone(&timer))
	{
		SpawnEnemies();
		StartTimer(&timer, enemySpawnTime);
	}
}

void DrawStuff()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);
	DrawRectangleRec(player.body, BLUE);
	
	for (size_t i = 0; i < bullets.size(); ++i)
		DrawRectangleRec(bullets[i].body, bullets[i].color);

	for (size_t i = 0; i < enemies.size(); ++i)
		DrawRectangleRec(enemies[i].body, enemies[i].color);

	DrawFPS(10, 10);

	EndDrawing();
}

void ProcessInput(float dt)
{
	if (IsKeyDown(KEY_W))
		player.body.y -= player.speed * dt;
	if (IsKeyDown(KEY_S))
		player.body.y += player.speed * dt;
	if (IsKeyDown(KEY_A))
		player.body.x -= player.speed * dt;
	if (IsKeyDown(KEY_D))
		player.body.x += player.speed * dt;

	if (IsKeyPressed(KEY_SPACE))
		SpawnBullet();
}

void CheckBounds()
{
	if ((player.body.x + player.body.width) >= GetScreenWidth())
		player.body.x = GetScreenWidth() - player.body.width;
	if (player.body.x <= 0)
		player.body.x = 0;
	if ((player.body.y + player.body.height) >= GetScreenHeight())
		player.body.y = GetScreenHeight() - player.body.height;
	if ((player.body.y) <= GetScreenHeight() / 2.f)
		player.body.y = GetScreenHeight() / 2.f;
}

void SpawnBullet()
{
	bullets.push_back(Bullet{ {player.body.x, player.body.y, 10.f, 10.f}, 1000.f, true, RED });
}

void UpdateBullets(float dt)
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (!bullets[i].active)
			continue;

		bullets[i].body.y -= bullets[i].speed * dt;
	}
}

void SpawnEnemies()
{
	bool validEnemyPos = false;
	float x{};
	float y{};

	while (!validEnemyPos)
	{
		x = RandomPosition().x;
		y = RandomPosition().y;
		Rectangle tempEnemy{ x, y, 50.f, 30.f };
		validEnemyPos = true;

		for (const auto& e : enemies)
		{
			if (CheckCollisionRecs(e.body, tempEnemy))
			{
				validEnemyPos = false;
				break;
			}
		}
	}

	enemies.push_back(Enemy{ {x, y, 50.f, 30.f}, 50.f, true, MAGENTA });
}

void UpdateEnemies(float dt)
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (!enemies[i].active)
			continue;

		enemies[i].body.y += enemies[i].speed * dt;
	}
}

Vector2 RandomPosition()
{
	std::random_device dev;
	static std::mt19937 rng(dev());

	std::uniform_real_distribution<float> distX(50.f, (float)GetScreenWidth() - 50.f);
	std::uniform_real_distribution<float> distY(-100.f, 0.f);

	return { distX(rng), distY(rng) };
}

void CheckCollisions()
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		for (size_t j = 0; j < bullets.size(); ++j)
		{
			if (enemies[i].active && bullets[j].active && CheckCollisionRecs(enemies[i].body, bullets[j].body))
			{
				enemies[i].active = false;
				bullets[j].active = false;
			}
		}
	}
}

void CleanUp()
{
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
		return !b.active;
		}), bullets.end());

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) {
		return !e.active;
		}), enemies.end());
}