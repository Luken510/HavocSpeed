#pragma once

#include <memory>

#include "window.h"
#include "modelLoader.h"
#include "PhysicsController.h"
#include "QuatCamera.h"
#include "eventHandler.h"


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
		static void HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods);
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
		/*!
		\brief Set up the lighting parameters
		*/
		void setmatricies(std::shared_ptr<GRAPHICS::Shader> Shader);
	
	private:
		std::shared_ptr<PHYSICS::PhysicsController> m_worldPhysics = std::make_shared<PHYSICS::PhysicsController>(PHYSICS::PhysicsController::getPhysicsInstance());
		std::shared_ptr<GRAPHICS::Model> m_car = nullptr;
		std::shared_ptr<GRAPHICS::Model> m_carRwheels = nullptr;
		std::shared_ptr<GRAPHICS::Model> m_carFrontR = nullptr;
		std::shared_ptr<GRAPHICS::Model> m_carFrontL = nullptr;
		std::shared_ptr<GRAPHICS::Model> m_map = nullptr;
		std::shared_ptr<UTIL::QuatCamera> m_camera = std::make_shared<UTIL::QuatCamera>();
		std::shared_ptr<GRAPHICS::Shader> m_objShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<GRAPHICS::Shader> m_MapShader = std::make_shared<GRAPHICS::Shader>();
		double m_timeSinceLastUpdate; //!< the time since the last update - "lag"
		double m_CurrentTime; //!< current time of update
		double m_PreviousTime; //!< previous time when updated
		glm::mat4 m_setModel;
		GAME::Window m_window; //!< Window Object to render the game
		UTIL::EventHandler &EventHandler = UTIL::EventHandler::getInstance(); // worth mentioning that this implementation won't work on multiple glfw windows
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
