#include "Player.h"

Player::Player(const PlayerSettings& ps)
	: _body{ ps.body }, _speed{ ps.speed }, _color{ ps.color }, _life{ ps.life }, _lives{ ps.lives },
	_score{ ps.score }, _collisionTimer{ ps.collisionTimer }, _vulnerable{ ps.vulnerable }
{

}

void Player::Update(Vector2 dir, float dt)
{
	_body.x += dir.x * _speed * dt;
	_body.y += dir.y * _speed * dt;
}

void Player::Draw() const
{
	DrawRectangleRec(_body, _color);
}
