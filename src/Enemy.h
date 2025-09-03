#ifndef ENEMY_H
#define ENEMY_H
#include "Entity.h"

class Enemy : public Entity
{
private:
	bool _active{};

public:
	Enemy(Rectangle body, float speed, Color color, bool active);

	void Update(float dt) override;
	void Draw() override;

	void SetActive(bool ac);
	bool IsActive() const;
	bool IsOffScreen() const;
};

#endif
