#ifndef ENTITY_H
#define ENTITY_H
#include "raylib.h"

class Entity
{
protected:
	Rectangle _body{};
	float _speed{};
	Color _color{};

	Entity(Rectangle b, float s, Color c) : _body{ b }, _speed{ s }, _color{ c } {}
	Entity(const Entity&) = default;
	Entity& operator=(const Entity&) = default;
	Entity(Entity&&) = default;
	Entity& operator=(Entity&&) = default;
	virtual ~Entity() = default;

public:
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

	const Rectangle GetBody() const { return _body; }
	void SetBody(const Rectangle& b) { _body = b; }
	const float GetSpeed() const { return _speed; }
	void SetSpeed(float s) { _speed = s; }
	const Color GetColor() const { return _color; }
	void SetColor(const Color& c) { _color = c; }
};

#endif
