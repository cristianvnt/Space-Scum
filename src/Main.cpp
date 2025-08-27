#include "raylib.h"

#include <vector>
#include <iostream>

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 700;

// bullet stuff
constexpr Vector2 bBody = { 10.f, 10.f };

constexpr int FPS = 60;

static void InitGame();
static void UpdateGame(float dt);
static void DrawStuff();
static void ProcessInput(float dt);
static void UpdateAndDraw(float dt);
static void CheckBounds();

static void SpawnBullet();
static void UpdateBullets(float dt);

struct Player
{
	Rectangle body{};
	float speed{};
};

struct Bullet
{
	Rectangle body{};
	float speed;
	bool active;
	Color color;
};

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BLABLEBLU");

	InitGame();

	SetTargetFPS(FPS);
	SetExitKey(KEY_Q);

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		UpdateAndDraw(dt);
	}

	CloseWindow();
}

Player player;
std::vector<Bullet> bullets;

void InitGame()
{
	player.body = { (float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 1.5f, 50.f, 70.f };
	player.speed = 200;
}

void UpdateGame(float dt)
{
	CheckBounds();
	ProcessInput(dt);
	UpdateBullets(dt);
}

void DrawStuff()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);
	DrawRectangleRec(player.body, BLUE);
	
	for (size_t i = 0; i < bullets.size(); ++i)
		DrawRectangleRec(bullets[i].body, bullets[i].color);

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

void UpdateAndDraw(float dt)
{
	UpdateGame(dt);
	DrawStuff();
}

void SpawnBullet()
{
	bullets.push_back(Bullet{ {player.body.x, player.body.y, bBody.x, bBody.y}, 100.f, true, RED });
}

void UpdateBullets(float dt)
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		if (!bullets[i].active)
			continue;

		bullets[i].body.y -= bullets[i].speed * dt;
		bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
			return b.body.y < -b.body.height; }), bullets.end());
	}
}