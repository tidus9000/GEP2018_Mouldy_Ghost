#pragma once
#include "Item.h"

class Character;

class Throwable: public Item
{
public:
	Throwable();
	Throwable(RenderData* _RD, std::string _filename);
	~Throwable();


	void Throw(Character* _player);
	virtual void pickUp(Character* _player);
	virtual void use(Character* _player) override;

	void setOnThrowString(std::string _throw) { m_on_throw = _throw; }
	void setOnHitPlayerString(std::string _hitplayer) { m_hit_player = _hitplayer; }
	void setOnHitGroundString(std::string _hitground) { m_hit_ground = _hitground; }

	std::string getOnThrowString() { return m_on_throw;}
	std::string getOnHitPlayerString() { return m_hit_player; }
	std::string getOnHitGroundString() { return m_hit_ground;}

	void CollisionEnter(Physics2D* _collision, Vector2 _normal);
	void Collision(Physics2D* _collision);

protected:
	Character * player_ignore = nullptr;

	std::string m_on_throw;
	std::string m_hit_player;
	std::string m_hit_ground;
	
	
};

