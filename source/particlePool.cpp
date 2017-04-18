#include "particlePool.h"





UTIL::ParticlePool::ParticlePool()
{
	//the first particle in the pool
	m_firstAvailable = &m_particles[0];

	//each particle points to the next for smooth iterations
	for (int i = 0; i < POOL_SIZE; i++)
	{
		m_particles[i].SetNext(&m_particles[i + 1]);
	}

	//the last member of the pool points to a null pointer (pool memory limit)
	m_particles[POOL_SIZE - 1].SetNext(NULL);

}

/*
Particle* ExampleParticle = ParticlePool.create(pos,vel,life);
delete ExamplleParticle;
*/

GRAPHICS::Particle* UTIL::ParticlePool::create(glm::vec3 position, glm::vec3 velocity, int lifetime)
{
	//make sure the current node isn't a null pointer aka end of the memory pool
	assert(m_firstAvailable != NULL);

	//remove it from the avalible list
	auto newParticle = m_firstAvailable;
	m_firstAvailable = newParticle->GetNext();

	newParticle->Init(position, velocity, lifetime);

	return newParticle;
}


void UTIL::ParticlePool::update()
{
	for (int i = 0; i < POOL_SIZE; i++)
	{
		if (m_particles[i].Update()) //if update returns true, aka it is in use
		{
			//then add this particle to the front of the current list.
			//Particle::update, should return a bool of true once completing its task
			// and thus the particle is now avalible for use, add to the list.
			//if Particle::update, should return false, then the particle is currently unused
			// assumd to be already in the available list.
			//called a free linked list.             
			if (!m_particles[i].Alive())
			{
				m_particles[i].SetNext(m_firstAvailable);
				m_firstAvailable = &m_particles[i];
			}
			
			
		}
	}

}


/*
// FOR THE DOCUMENTATION, EXAMPLES OF SOME OTHER POOLS
class MemoryPool
{// pointer to next class(struct)/ class(struct) to store complex object
	struct FreeStore
	{
		FreeStore *next;
	};
	//if pool needs to be expanded
	void expandSize();
	//when deleting the whole pool to give memory back to the CPU
	void StartDelete();
	//current
	FreeStore* freeStoreHead;
public:
	MemoryPool()
	{
		freeStoreHead = 0;
		expandSize();
	}
	virtual ~MemoryPool() {
		StartDelete();
	}
	virtual void* allocate(size_t);
	virtual void   free(void*);
};

MemoryPool gMemoryManager;

class Object
{
public:
	Object(double a, double b) : c(a), d(b) {}

	inline void* operator new(size_t);
	inline void   operator delete(void*);
private:
	double c; 
	double d; 
};


#define POOLSIZE 32

void MemoryPool::expandSize()
{
	//is the size of object > the size of free store, we need to reserve memory to the biggest of the two.
	size_t size = (sizeof(Object) > sizeof(FreeStore*)) ?
		sizeof(Object) : sizeof(FreeStore*);
	//Create a pointer for the start of the list - Object is just a char[] hence new Char;
	FreeStore* head = reinterpret_cast <FreeStore*> (new char[size]);
	//current beginning of new list
	freeStoreHead = head;
	//go through the new pool, setting its memory.
	for (int i = 0; i < POOLSIZE; i++) {
		head->next = reinterpret_cast <FreeStore*> (new char[size]);
		head = head->next;
	}
	//if the currents head = 0, at the end of the list
	head->next = 0;
}

inline void* MemoryPool::allocate(size_t size)
{
	//if the list is full, create a new one
	if (freeStoreHead == 0)
		expandSize();
	//set Head to the new Memory address
	FreeStore* head = freeStoreHead;
	//set the pointer to the next address, as the previous current
	freeStoreHead = head->next;
	return head;
}
*/