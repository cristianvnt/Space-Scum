#include "Bullet.h"
#include <Config.h>

Bullet::Bullet(Rectangle body, float speed, Color color, bool active)
	: Entity(body, speed, color), _active{ active }
{

}

void Bullet::Update(float dt)
{
	_body.y -= _speed * dt;
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

bool Bullet::IsOffScreen() const
{
	return _body.y + _body.height <= GAME::Settings::MARGIN;
}

