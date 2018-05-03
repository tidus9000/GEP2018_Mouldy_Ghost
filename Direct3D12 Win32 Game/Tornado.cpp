#include "pch.h"
#include "Tornado.h"
#include "SpawnHandler.h"


Tornado::Tornado(Vector2 _pos, RenderData * _RD, GameStateData * _GSD, SpawnHandler * _spawner, Vector2 _direction) : ImageGO2D(_RD, "tornado")
{
	m_spawner = _spawner;
#if _DEBUG
	m_physics = new VisiblePhysics(_RD);
#else
	m_physics = new Physics2D();
#endif
	m_pos = _pos;
	
	m_direction = _direction;

	Rectangle collider = Rectangle
	(m_pos.x - TextureSize().x, m_pos.y - TextureSize().y,
		TextureSize().x, TextureSize().y);

	m_physics->SetCollider(collider);

	m_physics->SetGrav(0);
	m_physics->SetOwner(this);

	tag = GameObjectTag::YO;
}

Tornado::~Tornado()
{
}

void Tornado::Tick(GameStateData * _GSD)
{
	m_physics->Tick(_GSD, m_pos);

	if (hit_rate > 0.2)
	{
		for (int i = 0; i < m_characters_in_tornado.size(); i++)
		{
			m_characters_in_tornado[i]->TakeDamage(2);
		
			m_characters_in_tornado[i]->Hit(Vector2(m_direction.x,-100), 100, nullptr);
		}
		hit_rate = 0;
	}

	hit_rate += _GSD->m_dt;
	m_elapsed_time += _GSD->m_dt;

	m_pos += 2 * m_direction*_GSD->m_dt;

	if (m_elapsed_time > 2)
	{
		m_spawner->onNotify(this, Event::OBJECT_DESTROYED);
	}
}

void Tornado::CollisionEnter(Physics2D * _collision, Vector2 _normal)
{
	if (_collision->GetOwner()->GetTag() == GameObjectTag::PLAYER)
	{
		Character* player = static_cast<Character*>(_collision->GetOwner());

		m_characters_in_tornado.push_back(player);

	}
}

void Tornado::Collision(Physics2D * _collision)
{
		
}


void Tornado::CollisionExit(Physics2D * _collision)
{
	if (_collision->GetOwner()->GetTag() == GameObjectTag::PLAYER)
	{
		Character* player = static_cast<Character*>(_collision->GetOwner());

		for (int i = 0; i < m_characters_in_tornado.size(); i++)
		{
			if (player == m_characters_in_tornado[i])
			{
				m_characters_in_tornado.erase(m_characters_in_tornado.begin() + i);
			}
		}
	}
}
