#include "Bullet.h"

void Bullet::Update(float dt)
{
	_body.x += GetVelocity().x * dt;
	_body.y += GetVelocity().y * dt;
}

void Bullet::Draw()
{
	DrawRectangleRec(_body, _color);
}

void Bullet::SetActive(bool ac)
{
	_active = ac;
}

bool Bullet::IsActive() const
{
	return _active;
}

