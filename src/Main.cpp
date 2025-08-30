#include "raylib.h"

#include <vector>
#include <iostream>
#include <random>
#include <string>

/*
*	game defines
*/
constexpr const int SCREEN_WIDTH = 1000;
constexpr const int SCREEN_HEIGHT = 700;
constexpr const float MARGIN = 70.f;
constexpr const int FPS = 60;

struct Timer;
struct Player;
struct Bullet;
struct Enemy;

/*
*	function defines
*/
static void InitGame();
static void Update(float dt);
static void Draw();
static void ProcessInput(float dt);
static void CheckBounds();
static void Restart();

static void SpawnBullet();
static void UpdateBullets(float dt);

static void SpawnEnemies();
static void UpdateEnemies(float dt);
static Vector2 RandomPosition();
static void CheckCollisions();
static void CleanUp();

// timer
static void StartTimer(Timer* timer, float lifeTime);
static void UpdateTimer(Timer* timer);
static bool TimerDone(Timer* timer);

struct Timer
{
	float _lifeTime;
};

struct Player
{
	Rectangle body{};
	Rectangle life{};
	int lives{};
	float speed{};
	int score{};
	Timer collisionTimer{};
	bool isVulnerable{};
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

struct UIBounds
{
	Rectangle topMargin{};
	Rectangle bottomMargin{};
};

enum class States
{
	MAIN_MENU = 0,
	GAMEPLAY,
	PAUSE,
	GAME_OVER
};

Player player;
Timer timer;

std::vector<Bullet> bullets;
std::vector<Enemy> enemies;

float enemySpawnTime = 0.7f;

States currState = States::MAIN_MENU;

UIBounds uiBounds;

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BLABLEBLU");

	InitGame();

	SetTargetFPS(FPS);
	SetExitKey(KEY_Q);

	StartTimer(&timer, enemySpawnTime);

	while (!WindowShouldClose())
	{
		Update(GetFrameTime());
		Draw();
	}

	CloseWindow();
}

#pragma region TIMER
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

void InitGame()
{
	uiBounds.topMargin = { 0.f, 0.f, (float)GetScreenWidth(), MARGIN };
	uiBounds.bottomMargin = { 0.f, (float)GetScreenHeight() - MARGIN, (float)GetScreenWidth(), MARGIN };

	player.body = { (float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 1.5f, 50.f, 70.f };
	player.speed = 500;
	player.isVulnerable = true;

	player.life = { (float)GetScreenWidth() - 50.f, 20.f, 30.f, 30.f };
	player.lives = 5;
}

void Update(float dt)
{
	if (IsKeyPressed(KEY_P))
	{
		if (currState == States::PAUSE)
			currState = States::GAMEPLAY;
		else if (currState == States::GAMEPLAY)
			currState = States::PAUSE;
	}

	if (IsKeyPressed(KEY_R))
		Restart();

	switch (currState)
	{
	case States::MAIN_MENU:
		if (IsKeyPressed(KEY_ENTER))
			currState = States::GAMEPLAY;
		break;
	case States::GAMEPLAY:
		if (!player.lives)
		{
			currState = States::GAME_OVER;
			return;
		}

		UpdateTimer(&timer);
		UpdateTimer(&player.collisionTimer);
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

		if (TimerDone(&player.collisionTimer))
			player.isVulnerable = true;
		break;
	case States::PAUSE:
		break;
	case States::GAME_OVER:
		break;
	default:
		break;
	}
}

void Draw()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);

	switch (currState)
	{
	case States::MAIN_MENU:
		DrawText("Press ENTER to START GAME", 100, 300, 50, BLACK);
		break;
	case States::GAMEPLAY:
		DrawRectangleRec(player.body, BLUE);

		for (size_t i = 0; i < bullets.size(); ++i)
			DrawRectangleRec(bullets[i].body, bullets[i].color);

		for (size_t i = 0; i < enemies.size(); ++i)
			DrawRectangleRec(enemies[i].body, enemies[i].color);

		DrawRectangleRec(uiBounds.topMargin, LIGHTGRAY);

		for (size_t i = 0; i < player.lives; ++i)
			DrawRectangleRec({ player.life.x - i * 40, player.life.y, player.life.width, player.life.height }, GREEN);

		DrawRectangleRec(uiBounds.bottomMargin, LIGHTGRAY);
		DrawText(("Score: " + std::to_string(player.score)).c_str(), 20, GetScreenHeight() - 50, 50, BLACK);

		DrawFPS(10, 10);
		break;
	case States::PAUSE:
		DrawText("PAUSED", 350, 200, 60, BLACK);
		DrawText("Press P to RESUME", 200, 300, 50, BLACK);
		break;
	case States::GAME_OVER:
		DrawText("GAME OVER", 300, 200, 60, BLACK);
		DrawText("Press R to RESTART", 200, 300, 50, BLACK);
		break;
	default:
		break;
	}

	EndDrawing();
}

void ProcessInput(float dt)
{
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		player.body.y -= player.speed * dt;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		player.body.y += player.speed * dt;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		player.body.x -= player.speed * dt;
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		player.body.x += player.speed * dt;

	if (IsKeyPressed(KEY_SPACE))
		SpawnBullet();
}

void CheckBounds()
{
	if ((player.body.x + player.body.width) >= (float)GetScreenWidth())
		player.body.x = (float)GetScreenWidth() - player.body.width;
	if (player.body.x <= 0)
		player.body.x = 0;
	if ((player.body.y + player.body.height) >= (float)GetScreenHeight() - MARGIN)
		player.body.y = (float)GetScreenHeight() - MARGIN - player.body.height;
	if ((player.body.y) <= (float)GetScreenHeight() / 2.f)
		player.body.y = (float)GetScreenHeight() / 2.f;
}

void Restart()
{
	InitGame();

	enemies.clear();
	bullets.clear();

	currState = States::MAIN_MENU;
	player.score = 0;
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

		if (bullets[i].body.y + bullets[i].body.height <= MARGIN)
			bullets[i].active = false;
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

	enemies.push_back(Enemy{ {x, y, 50.f, 30.f}, 80.f, true, MAGENTA });
}

void UpdateEnemies(float dt)
{
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (!enemies[i].active)
			continue;

		enemies[i].body.y += enemies[i].speed * dt;

		if (!player.lives)
		{
			currState = States::GAME_OVER;
			return;
		}

		if (CheckCollisionRecs(enemies[i].body, player.body) && player.lives)
		{
			if (player.isVulnerable)
			{
				player.lives -= 1;
				StartTimer(&player.collisionTimer, 0.5f);
				player.isVulnerable = false;
			}
		}

		if (enemies[i].body.y >= uiBounds.bottomMargin.y && player.lives)
		{
			enemies[i].active = false;
			player.lives -= 1;
		}
	}
}

Vector2 RandomPosition()
{
	std::random_device dev;
	static std::mt19937 rng(dev());

	std::uniform_real_distribution<float> distX(50.f, (float)GetScreenWidth() - 50.f);
	std::uniform_real_distribution<float> distY(-50.f, 0.f);

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
				player.score += 10;
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