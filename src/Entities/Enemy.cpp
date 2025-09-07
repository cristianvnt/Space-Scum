#include "Enemy.h"
#include "utils/Config.h"

void Enemy::Update(float dt)
{
	_body.x += GetVelocity().x * dt;
	_body.y += GetVelocity().y * dt;
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
