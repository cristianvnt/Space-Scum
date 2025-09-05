#ifndef BULLET_H
#define BULLET_H
#include "Entity.h"

class Bullet : public Entity
{
private:
	bool _active{};

public:
	Bullet(Rectangle body, Vector2 v, Color color, bool active) : Entity(body, v, color), _active{ active } {}

	void Update(float dt) override;
	void Draw() override;

	void SetActive(bool ac);
	bool IsActive() const;
	bool IsOffScreen() const;
};

#endif
