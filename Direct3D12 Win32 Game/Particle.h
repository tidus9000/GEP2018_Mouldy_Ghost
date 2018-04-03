#pragma once
#include "GameObject2D.h"
class Particle :
	public ImageGO2D
{
public:
	Particle();
	Particle(Vector2 _origin, std::string _filename, RenderData * _RD);
	~Particle();
	virtual void Tick(GameStateData* _GSD) override;
	//virtual void Render(RenderData * _RD, int _sprite,
	//	Vector2 _cam_pos = Vector2::Zero, float _zoom = 1) override;
	void setDestination(Vector2 _destination);
	void setSpeed(float _speed);
	void setLifetime(float _lifetime);
	bool getDead();
private:
	Vector2 m_origin;
	Vector2 m_destination;
	Vector2 m_direction;
	bool m_dead = false;
	bool m_fade = false;
	float m_speed;
	float m_lifetime;
	float m_elapsed_time = 0;
};
