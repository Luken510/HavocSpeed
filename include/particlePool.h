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

TObject pool_[POOL_SIZE];
bool    inUse_[POOL_SIZE];
};

*/

#define MEMORY_POOL_SIZE 100

class Particle; // need to set this up and include the header file;

namespace UTIL
{
	class ParticlePool
	{
	public:
		
		ParticlePool();

		//returns a pointer to the class, for outside initializing
		Particle* create(double x, double y, double xVel, double yVel, int duration);
		// just temp stuff for now

		void update();


	private:
		static const int POOL_SIZE = MEMORY_POOL_SIZE;
		Particle m_particles[MEMORY_POOL_SIZE];
		Particle* m_firstAvailable;

	};
}