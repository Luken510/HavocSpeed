#include "particlePool.h"





UTIL::ParticlePool::ParticlePool()
{
	//the first particle in the pool
	m_firstAvailable = &m_particles[0];

	//each particle points to the next for smooth iterations
	for (int i = 0; i < POOL_SIZE; i++)
	{
		m_particles[i].setNext(&m_particles[i + 1]);
	}

	//the last member of the pool points to a null pointer (pool memory limit)
	m_particles[POOL_SIZE - 1].setNext(NULL);

}

Particle* UTIL::ParticlePool::create(double x, double y, double xVel, double yVel, int duration)
{
	//make sure the current node isn't a null pointer aka end of the memory pool
	assert(m_firstAvailable != NULL);

	//remove it from the avalible list
	Particle* newParticle = m_firstAvailable;
	m_firstAvailable = newParticle->getNext();

	return newParticle;
}


void UTIL::ParticlePool::update()
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (m_particles[i].update()) //if update returns true, aka it is in use
		{
			//then add this particle to the front of the current list.
			//Particle::update, should return a bool of true once completing its task
			// and thus the particle is now avalible for use, add to the list.
			//if Particle::update, should return false, then the particle is currently unused
			// assumd to be already in the available list.

			//called a free linked list.
			m_particles[i].setNext(m_firstAvailable);
			m_firstAvailable = &m_particles[i];
		}
	}

}
