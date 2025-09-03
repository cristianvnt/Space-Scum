#include "Enemy.h"
#include <Config.h>

Enemy::Enemy(Rectangle body, float speed, Color color, bool active)
	: Entity(body, speed, color), _active{ active }
{

}

void Enemy::Update(float dt)
{
	_body.y += _speed * dt;
}

void Enemy::Draw()
{
	DrawRectangleRec(_body, _color);
}

void Enemy::SetActive(bool ac)
{
	_active = ac;
}

bool Enemy::IsActive() const
{
	return _active;
}

bool Enemy::IsOffScreen() const
{
	return _body.y >= GAME::Settings::SCREEN_HEIGHT - GAME::Settings::MARGIN;
}
