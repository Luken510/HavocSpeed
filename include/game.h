#pragma once

#include <memory>

#include "window.h"
#include "modelLoader.h"
#include "PhysicsController.h"


#define MS_PER_UPDATE (float)0.0166666666666667

namespace GAME {

	class Game {
	public:
		/*
		\brief Constructor
		*/
		Game();
		/*
		\brief Destructor
		*/
		~Game();
		/*
		\brief Initailise the game
		*/
		void Init();
		/*
		\brief Run the main Game Loop
		*/
		void RunGame();
		/*
		\brief Handle Input events
		\param window, window that is looking for inputs
		\param key, current key that is calling an action
		\param scancode unique code for each key
		\param action, the action the key is doing, REPEAT, PRESS, or RELEASE
		\param mods, 
		*/
		static void HandleEvents(GLFWwindow* window, int key, int scancode, int action, int mods);
		/*
		\brief Update the game
		\param deltaTime, Current timestep within the game
		*/
		void Update(float deltaTime);
		/*
		\brief Render the Game
		\param Interpolate, used for buffering the render mid update if needed
		*/
		void Render(float Interpolate);
	private:
		std::shared_ptr<PHYSICS::PhysicsController> worldPhysics = nullptr;
		std::shared_ptr<GRAPHICS::Model> car = nullptr;
		std::shared_ptr<GRAPHICS::Shader> shader = nullptr;
		double m_timeSinceLastUpdate; //!< the time since the last update - "lag"
		double m_CurrentTime; //!< current time of update
		double m_PreviousTime; //!< previous time when updated
		GAME::Window m_window; //!< Window Object to render the game

		//Physics setup
		//btBroadphaseInterface* broadphase;

		//btDefaultCollisionConfiguration* collisionConfig;
		//btCollisionDispatcher* dispatcher;

		// the actual physics solver
		//btSequentialImpulseConstraintSolver* solver;

		// the world
//		btDiscreteDynamicsWorld* dynamicsWorld;
	};
}
