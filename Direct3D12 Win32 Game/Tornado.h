#pragma once
#include "pch.h"
#include "ImageGO2D.h"
#include "VisiblePhysics.h"

class Tornado : public ImageGO2D
{
public:
	Tornado() = default;
	Tornado(Vector2 _pos, RenderData* _RD, GameStateData* _GSD,
		SpawnHandler * _spawner, Vector2 _direction);
	~Tornado();

	virtual void Tick(GameStateData* _GSD);

	virtual void CollisionEnter
	(Physics2D* _collision, Vector2 _normal) override;
	virtual void Collision
	(Physics2D* _collision) override;
	virtual void CollisionExit
	(Physics2D* _collision) override;

private:

	float hit_rate = 0;
	Vector2 m_direction;
	float m_elapsed_time = 0;

	SpawnHandler* m_spawner = nullptr;

	std::vector<Character*> m_characters_in_tornado;
};