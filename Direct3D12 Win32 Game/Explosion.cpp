#include "pch.h"
#include "Explosion.h"
#include "GameStateData.h"
#include "VisiblePhysics.h"
#include "SpawnHandler.h"

Explosion::Explosion(Vector2 _pos, RenderData * _RD, 
	GameStateData * _GSD, SpawnHandler * _spawner, 
	float _exp_radius, float _power):ImageGO2D(_RD,"explosion")
{
	m_spawner = _spawner;
#if _DEBUG
	m_physics = new VisiblePhysics(_RD);
#else
	m_physics = new Physics2D();
#endif
	m_pos = _pos;
	//m_spawner->onNotify(this, Event::OBJECT_INSTANTIATED);

	/*Rectangle rect = Rectangle(m_pos.x - _exp_radius/2, m_pos.y - _exp_radius / 2,
		m_pos.x + _exp_radius / 2, m_pos.y+ _exp_radius / 2);*/

	Rectangle collider = Rectangle
	(m_pos.x - TextureSize().x / 3, m_pos.y - TextureSize().y / 3,
		TextureSize().x, TextureSize().y);
	
	m_physics->SetCollider(collider);

	//m_scale = Vector2(0.1, 0.1);
	
	m_physics->SetGrav(0);
	m_physics->SetOwner(this);

	tag = GameObjectTag::YO;
}

Explosion::~Explosion()
{
}

void Explosion::Tick(GameStateData * _GSD)
{
	if (elapsed_time < 0.1)
	{
		m_physics->Tick(_GSD, m_pos);
	}
	elapsed_time += _GSD->m_dt;

	if (elapsed_time > 1.5)
	{
		m_spawner->onNotify(this, Event::OBJECT_DESTROYED);
	}
}

void Explosion::CollisionEnter(Physics2D * _collision, Vector2 _normal)
{

	if (_collision->GetOwner()->GetTag() == GameObjectTag::PLAYER)
	{
		Character* player = static_cast<Character*>(_collision->GetOwner());

		Vector2 dir = player->GetPos() - m_pos;
		dir.Normalize();

		//debug values
		player->TakeDamage(20);
		player->Hit(dir, 200000, nullptr);
	}
}

void Explosion::Collision(Physics2D * _collision)
{
	int i = 0;
}