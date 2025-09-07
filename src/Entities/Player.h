#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "utils/Timer.h"
#include "Entity.h"

struct PlayerSettings
{
	Rectangle body{};
	float speed{};
	Vector2 velocity{};
	Color color{};
	Rectangle life{};
	int lives{};
	int score{};
	Timer collisionTimer{};
	bool vulnerable{};
};

class Player
{
private:
	Rectangle _body{};
	float _speed{};
	Vector2 _velocity{};
	Color _color{};
	Rectangle _life{};
	int _lives{};
	int _score{};
	Timer _collisionTimer{};
	bool _vulnerable{};

public:
	Player(const PlayerSettings& ps);

	void Update(float dt);
	void Draw() const;

	Vector2 GetVelocity() const;
	void SetVelocity(Vector2 velocity);
	void DefaultVelocity();

	const Rectangle GetBody() const { return _body; }
	void SetBody(const Rectangle& b) { _body = b; }
	void SetBodyVec(const Vector2& v) { _body.x = v.x; _body.y = v.y; }

	const float GetSpeed() const { return _speed; }
	void SetSpeed(float s) { _speed = s; }

	const Color GetColor() const { return _color; }
	void SetColor(const Color& c) { _color = c; }

	const Rectangle GetLife() const { return _life; }
	void SetLife(const Rectangle& r) { _life = r; }

	const int GetLives() const { return _lives; }
	void SetLives(int l) { _lives = l; }

	const int GetScore() const { return _score; }
	void SetScore(int s) { _score = s; }

	Timer& GetCollisionTimer() { return _collisionTimer; }
	const Timer& GetCollisionTimer() const { return _collisionTimer; }
	void SetCollisionTimer(const Timer& ct) { _collisionTimer = ct; }

	const bool IsVulnerable() const { return _vulnerable; }
	void SetVulnerable(bool v) { _vulnerable = v; }
};

#endif
