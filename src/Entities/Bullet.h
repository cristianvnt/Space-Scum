#ifndef BULLET_H
#define BULLET_H
#include "Entity.h"

struct BulletSettings
{
	Rectangle body{};
	Vector2 velocity{};
	Color color{};
	float speed{};
	Vector2 direction{};
	bool active{};

	BulletSettings& SetBody(const Rectangle& b) { body = b; return *this; }
	BulletSettings& SetVelocity(Vector2 v) { velocity = v; return *this; }
	BulletSettings& SetColor(Color c) { color = c; return *this; }
	BulletSettings& SetSpeed(float s) { speed = s; return *this; }
	BulletSettings& SetDirection(Vector2 d) { direction = d; return *this; }
	BulletSettings& SetActive(bool a) { active = a; return *this; }
};

class Bullet : public Entity
{
private:
	float _speed{};
	Vector2 _direction{};
	bool _active{};

public:
	Bullet(const BulletSettings& bs) : Entity(bs.body, bs.velocity, bs.color), _speed{ bs.speed }, _direction{ bs.direction }, _active { bs.active } {}

	void Update(float dt) override;
	void Draw() override;

	void SetActive(bool ac);
	bool IsActive() const;
};

#endif
