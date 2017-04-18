#include "particle.h"

void GRAPHICS::Particle::Init(glm::vec3 Position, glm::vec3 Velocity, int lifetime)
{
	 m_state.live.m_life = lifetime;

	 m_state.live.m_position[0] = Position.x;
	 m_state.live.m_position[1] = Position.y;
	 m_state.live.m_position[2] = Position.z;

	 m_state.live.m_velocity[0] = Velocity.x;
	 m_state.live.m_velocity[1] = Velocity.y;
	 m_state.live.m_velocity[2] = Velocity.z;
}

bool GRAPHICS::Particle::Update()
{
	if (!Alive())
		return false;

	m_state.live.m_life--;
	//do pos and vel stuff
		
	
	
	return true;
}

GRAPHICS::Particle* GRAPHICS::Particle::GetNext() const
{
	return m_state.next; 
}

void GRAPHICS::Particle::SetNext(Particle * next)
{
	 m_state.next = next; 
}

bool GRAPHICS::Particle::Alive() const
{
	 return m_state.live.m_life > 0;
}

