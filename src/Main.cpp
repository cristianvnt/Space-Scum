#include "raylib.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr float RADIUS = 20.f;

constexpr int FPS = 60;

static void InitGame();
static void UpdateGame(float dt);
static void DrawStuff();
static void ProcessInput(float dt);
static void UpdateAndDraw(float dt);
static void CheckBounds();

Vector2 player = { 0, 0 };

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

void InitGame()
{
	player = { (float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 1.5f };
}

void UpdateGame(float dt)
{
	CheckBounds();
	ProcessInput(dt);
}

void DrawStuff()
{
	BeginDrawing();

	ClearBackground(RAYWHITE);
	DrawCircleV(player, RADIUS, BLUE);
	DrawFPS(10, 10);

	EndDrawing();
}

void ProcessInput(float dt)
{
	if (IsKeyDown(KEY_W))
		player.y -= 100.f * dt;
	if (IsKeyDown(KEY_S))
		player.y += 100.f * dt;
	if (IsKeyDown(KEY_A))
		player.x -= 100.f * dt;
	if (IsKeyDown(KEY_D))
		player.x += 100.f * dt;
}

void CheckBounds()
{
	if (player.y < (float)GetScreenHeight() / 2.f)
		player.y = (float)GetScreenHeight() / 2.f;
	if (player.y > (float)GetScreenHeight() - RADIUS)
		player.y = (float)GetScreenHeight() - RADIUS;
	if (player.x > (float)GetScreenWidth() - RADIUS)
		player.x = (float)GetScreenWidth() - RADIUS;
	if (player.x < RADIUS)
		player.x = RADIUS;
}

void UpdateAndDraw(float dt)
{
	UpdateGame(dt);
	DrawStuff();
}