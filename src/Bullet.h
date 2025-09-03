#ifndef BULLET_H
#define BULLET_H
#include "Entity.h"

class Bullet : public Entity
{
private:
	bool _active{};

public:
	Bullet(Rectangle body, float speed, Color color, bool active);

	void Update(float dt) override;
	void Draw() override;

	void SetActive(bool ac);
	bool IsActive() const;
	bool IsOffScreen() const;
};

#endif
