#pragma once
#include <glm\glm.hpp>

namespace  GRAPHICS {

	class Particle {
	public:

		void Init(glm::vec3 Position, glm::vec3 Velocity, int lifetime);

		bool Update();

		Particle* GetNext() const;

		void SetNext(Particle* next);

		bool Alive() const;

	private:
		//creates a default constructor
		//saving memory
		// if not in use, only contains pointer to its next iteration
		//if in use, has position and velocity.
		union 
		{
			struct 
			{
				int m_life;
				double m_position[3];
				double m_velocity[3];
			}live;

			Particle* next;
		}m_state;
	};
}