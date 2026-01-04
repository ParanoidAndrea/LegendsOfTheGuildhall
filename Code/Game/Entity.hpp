#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Render/Renderer.hpp"
#include "GameCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
class Game;

class Entity
{
public:
	Entity(Game* owner, Vec2 const& startPos);
	virtual ~Entity();

	virtual void Update(float deltaSeconds) = 0 ; // subclass must implement the class
	virtual void Render() const = 0;
	//virtual void DebugRender() const;
	Game* m_game;
	Vec2  m_position;
	Vec2  m_velocity;
	float m_orientationDegrees = 0.f;
	float m_physicsRadius = 0.f;
	float m_cosmeticRadius = 0.f;
	int   m_health=ENTITIES_HEALTH;
	bool  m_isGarbage = false;
	bool  m_isDead = false;
	//bool  isOffScreen(Vec2 const& position, float radius) const;
	Vec2 const GetForwardNormal() const;
	bool  IsAlive();
	void CheckDeathStatus();
	//Vec2 RandomOffscreen() ;
	//Game* m_game = new Game();
	
};


