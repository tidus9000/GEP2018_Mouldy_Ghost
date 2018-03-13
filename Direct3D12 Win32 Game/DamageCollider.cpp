#include "pch.h"
#include "DamageCollider.h"
#include "Physics2D.h"

DamageCollider::DamageCollider(DamageColData _data)
{
	m_physics = new Physics2D();
	tag = GameObjectTag::ATTACK;
	m_data = _data;
	if (_data.child_to_player)
	{
		parent = m_data.user;
	}
}

DamageCollider::~DamageCollider()
{
	delete m_physics;
	m_physics = nullptr;
}

void DamageCollider::CollisionEnter(Physics2D * _collision, Vector2 _normal)
{
	if (_collision->GetOwner() != m_data.user)
	{
		GameObjectTag collision_tag = _collision->GetOwner()->GetTag();
		if (collision_tag == GameObjectTag::PLAYER)
		{
			Character* target = static_cast<Character*>(_collision->GetOwner());
			target->TakeDamage(m_data.damage);
			target->GetHit(_normal, m_data.knockback);

			if (m_data.contact == Destroy::ON_PLAYER_HIT)
			{
				//TODO: destroy object event which removes it from the gameobject2ds vector, the collider vector and frees the memory
			}
		}
		if (m_data.contact == Destroy::ON_ANYTHING_HIT)
		{
			//destroy it using above event
		}
	}
}