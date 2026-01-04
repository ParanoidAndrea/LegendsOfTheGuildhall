#include "Entity.hpp"
Entity::Entity(Game* owner, Vec2 const& startPos)
	:m_game(owner), m_position(startPos)
{

}

Entity::~Entity()
{

}

// void Entity::DebugRender() const
// {
// 	Vec2 fwd = Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees))* m_cosmeticRadius;
// 	DebugDrawLine(m_position, m_position + fwd, 0.2f, Rgba8(255, 0, 0, 255));
// 	Vec2 relative_Left = fwd.GetRotated90Degrees();
// 	DebugDrawLine(m_position, m_position + relative_Left, 0.2f, Rgba8(0, 255, 0, 255));
// 	DebugDrawRing(m_position, m_cosmeticRadius, 0.3f, Rgba8(255, 0, 255, 255));
// 	DebugDrawRing(m_position, m_physicsRadius, 0.3f, Rgba8(0, 255, 255, 255));
// 	DebugDrawLine(m_position, m_position + m_velocity, 0.2f, Rgba8(255, 255, 0, 255));
// }

// bool Entity::isOffScreen(Vec2 const& m_postion, float radius) const
// {
// 	if ((m_postion.x + radius >= WORLD_SIZE_X )|| (m_postion.x - radius <= 0)
//     ||  (m_postion.y + radius >= WORLD_SIZE_Y) || (m_postion.y - radius <= 0) )
// 		return true;
// 	return false;
// }
// Vec2 Entity::RandomOffscreen() 
// {
// 	float random = g_rng->RollRandomFloatZeroToOne();
// 	if (random < 0.25)
// 	{
// 		m_position.x = -m_cosmeticRadius;
// 		m_position.y = g_rng->RollRandomFloatInRange(0, WORLD_CENTER_Y + m_cosmeticRadius);
// 	}
// 	else if (random >= 0.25 && random< 0.5)
// 	{
// 		m_position.x = WORLD_SIZE_X+ m_cosmeticRadius;
// 		m_position.y = g_rng->RollRandomFloatInRange(-m_cosmeticRadius, WORLD_CENTER_Y);
// 	}
// 	else if (random >= 0.5 && random < 0.75)
// 	{
// 		m_position.x = g_rng->RollRandomFloatInRange(-m_cosmeticRadius, WORLD_SIZE_X);
// 		m_position.y = -m_cosmeticRadius;
// 	}
// 	else 
// 	{
// 		m_position.x = g_rng->RollRandomFloatInRange(0, WORLD_SIZE_X + m_cosmeticRadius);
// 		m_position.y = WORLD_SIZE_Y+ m_cosmeticRadius;
// 	}
// 	return m_position;
// }

Vec2 const Entity::GetForwardNormal() const
{
	return Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees));
}

void Entity::CheckDeathStatus()
{
	if (m_health <= 0)
	{
		m_isDead = true;
	}
}

bool Entity::IsAlive()
{
	if (m_isGarbage)
	{
		return false;
	}
	 return true; 
}
