#include "Player.h"

#include <cmath>
#include <raymath.h>

Player::Player(const PlayerSettings& ps)
	: _body{ ps.body }, _speed{ ps.speed }, _velocity{ps.velocity.x, ps.velocity.y }, _color{ ps.color }, _life{ ps.life }, _lives{ ps.lives },
	_score{ ps.score }, _collisionTimer{ ps.collisionTimer }, _vulnerable{ ps.vulnerable }
{

}

void Player::Update(float dt)
{
	_body.x += GetVelocity().x * dt;
	_body.y += GetVelocity().y * dt;
}

void Player::Draw() const
{
	DrawRectangleRec(_body, _color);
}

Vector2 Player::GetVelocity() const
{
	return { _speed * _velocity.x, _speed * _velocity.y };
}

void Player::SetVelocity(Vector2 velocity)
{
	_speed = Vector2Length(velocity);
	_velocity = Vector2Normalize(velocity);
}

void Player::DefaultVelocity()
{
	_velocity.x = 0.f;
	_velocity.y = 0.f;
}
