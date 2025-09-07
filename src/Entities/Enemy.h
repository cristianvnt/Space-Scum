#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

struct EnemySettings
{
	Rectangle body{};
	Vector2 velocity{};
	Color color{};
	float spawnTime{};
	bool active{};
	float speed{};
};

class Enemy : public Entity
{
private:
	float _speed{};
	bool _active{};
	float _spawnTime{};

public:
	Enemy(const EnemySettings& es) : Entity(es.body, es.velocity, es.color), _speed{ es.speed }, _active{ es.active }, _spawnTime{ es.spawnTime } {}

	void Update(float dt) override;
	void Draw() override;

	void SetActive(bool ac);
	bool IsActive() const;
};

#endif
