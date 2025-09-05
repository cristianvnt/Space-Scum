#ifndef ENTITY_H
#define ENTITY_H
#include "raylib.h"

class Entity
{
protected:
	Rectangle _body{};
	Vector2 _velocity{};
	Color _color{};

	Entity(Rectangle b, Vector2 v, Color c) : _body{ b }, _velocity { v }, _color{ c } {}
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
	Vector2 GetVelocity() const { return _velocity; }
	void SetVelocity(Vector2 velocity) { _velocity = velocity; }
	const Color GetColor() const { return _color; }
	void SetColor(const Color& c) { _color = c; }
};

#endif
