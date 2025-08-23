#include "raylib.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

constexpr int FPS = 60;

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BLABLEBLU");
	SetTargetFPS(FPS);
	
	Vector2 ball = { (float)SCREEN_WIDTH / 2.f, (float)SCREEN_HEIGHT / 2.f };

	while (!WindowShouldClose())
	{
		// update vars here
		if (IsKeyDown(KEY_W))
			ball.y -= 3;
		if (IsKeyDown(KEY_S))
			ball.y += 3;
		if (IsKeyDown(KEY_D))
			ball.x += 3;
		if (IsKeyDown(KEY_A))
			ball.x -= 3;

		// draw
		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawCircleV(ball, 10.f, BLUE);

		EndDrawing();
	}

	CloseWindow();
}