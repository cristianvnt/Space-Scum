#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

struct EnemySettings
{
	Rectangle body{};
	Vector2 velocity{};
	Color color{};
	float speed{};
	Vector2 direction{};
	float spawnTime{};
	bool active{};

	EnemySettings& SetBody(const Rectangle& b) { body = b; return *this; }
	EnemySettings& SetVelocity(Vector2 dir, float s) { direction = dir; speed = s; velocity = { s * dir.x, s * dir.y }; return *this; }
	EnemySettings& SetColor(Color c) { color = c; return *this; }
	EnemySettings& SetSpeed(float s) { speed = s; velocity = { s * direction.x, s * direction.y }; return *this; }
	EnemySettings& SetDirection(Vector2 d) { direction = d; velocity = { speed * d.x, speed * d.y }; return *this; }
	EnemySettings& SetSpawnTime(float st) { spawnTime = st; return *this; }
	EnemySettings& SetActive(bool a) { active = a; return *this; }
};

class Enemy : public Entity
{
private:
	float _speed{};
	Vector2 _direction{};
	float _spawnTime{};
	bool _active{};
	
public:
	Enemy(const EnemySettings& es) : Entity(es.body, es.velocity, es.color), _speed{ es.speed }, _direction{ es.direction }, _active { es.active }, _spawnTime{ es.spawnTime } {}

	void Update(float dt) override;
	void Draw() override;

	void SetActive(bool ac);
	bool IsActive() const;
};

#endif
