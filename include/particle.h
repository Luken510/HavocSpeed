#pragma once
#include <glm\glm.hpp>

namespace  GRAPHICS {

	class Particle {
	public:
		/*!
		\brief Init function, used to initialise a particle.
		\param Position the position of the particle.
		\param Velocity the velocity of the particle.
		\param lifetime the duration the particle lives before deleted.
		*/
		void Init(glm::vec3 Position, glm::vec3 Velocity, int lifetime);

		/*!
		\brief Update function, used to process particles movements and death.
		*/
		bool Update();
		/*!
		\brief GetNext function to return a pointer to the next in line Particle
		*/
		Particle* GetNext() const;
		/*!
		\brief Set Next
		\param next sets next particle in the linked list.
		*/
		void SetNext(Particle* next);
		/*!
		\brief Alive a boolean to determine if the particle is alive.
		*/
		bool Alive() const;

	private:
		//creates a default constructor
		//saving memory
		// if not in use, only contains pointer to its next iteration
		//if in use, has position and velocity.
	
		/*! \union m_state
		 \brief m_state  if not in use, only contains pointer to its next iteration //if in use, has position and velocity.
		*/
		union 
		{
			struct 
			{
				int m_life; //!< how long the particle lives.
				double m_position[3]; //!< the position of the particle.
				double m_velocity[3]; //!< the velocity of the particle.
			}live;

			Particle* next; //!< Pointer to the next in list.
		}m_state;
	};
}