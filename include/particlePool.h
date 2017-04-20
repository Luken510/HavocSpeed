#pragma once
/*
Can create a generic object pool, but for our system we only need one for the particle and hence a specifc
pool is created, for a more adaptible code, you'd want the generic one of course, but that does 
bring the downside of having to account for any object, and thus less effiecent 

#define MEMORY_POOL_SIZE 100;

template <class TObject>
class GenericPool
{
private:
static const int POOL_SIZE = MEMORY_POOL_SIZE;

TObject pool[POOL_SIZE];
bool    inUse[POOL_SIZE];
};

*/
#include <glm\glm.hpp>
#include "particle.h"

#define MEMORY_POOL_SIZE 100

namespace UTIL
{
	class ParticlePool
	{
	public:
		/*!
		\brief Constructor
		*/
		ParticlePool();

		/*!
		\brief create used to allocate memory from the Object Pool
		\return a pointer to the currently created particle.
		*/
		GRAPHICS::Particle* create(glm::vec3 position, glm::vec3 velocity, int lifetime);
		/*!
		\brief Update function used to update all active Objects in the pool or deallocate if dead.
		*/
		void update();

	private:
		static const int POOL_SIZE = MEMORY_POOL_SIZE; //!< Object Pool size
		GRAPHICS::Particle m_particles[POOL_SIZE];//!< allocated memory at the start (size of the pool)
		GRAPHICS::Particle* m_firstAvailable;//!< Pointer to the first avaliable of the list.

	};
}
