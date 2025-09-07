#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

class Enemy : public Entity
{
private:
	bool _active{};
	float _spawnTime{};

public:
	Enemy(Rectangle body, Vector2 v, Color color, bool active, float spawnTime) : Entity(body, v, color), _active{ active }, _spawnTime{ spawnTime } {}

	void Update(float dt) override;
	void Draw() override;

	void SetActive(bool ac);
	bool IsActive() const;
	bool IsOffScreen() const;
};

#endif
