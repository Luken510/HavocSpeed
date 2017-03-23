#pragma once

#include <memory>

#include "window.h"
#include "modelLoader.h"
#include "PhysicsController.h"
#include "QuatCamera.h"
#include "eventHandler.h"
#include "raceCar.h"


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
		\brief Update the game
		\param deltaTime, Current timestep within the game
		*/
		void Update(double deltaTime);
		/*
		\brief Render the Game
		\param Interpolate, used for buffering the render mid update if needed
		*/
		void Render(double Interpolate);
		/*!
		\brief Set up the lighting parameters
		*/
		void setmatricies(std::shared_ptr<GRAPHICS::Shader> Shader);
		void WireFrameMode(std::vector<GRAPHICS::Line> & lines);
	
	private:
		GAME::Window m_window; //!< Window Object to render the game
		
		//std::shared_ptr<PHYSICS::PhysicsController> m_worldPhysics = m_worldPhysics = std::make_shared<PHYSICS::PhysicsController>(PHYSICS::PhysicsController::GetPhysicsInstance());
		std::shared_ptr<UTIL::QuatCamera> m_camera = std::make_shared<UTIL::QuatCamera>(glm::vec3(0.0f,30.0f,100.0f));
		
		std::shared_ptr<GRAPHICS::Shader> m_objShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<GRAPHICS::Shader> m_MapShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<GRAPHICS::Shader> m_debugDrawerShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<GRAPHICS::Model> m_map = nullptr;
		std::shared_ptr<RaceCar> m_player1Car = nullptr; // if it isn't a pointer, make sure the window is init'd before called this object, as it will return an error
		double m_timeSinceLastUpdate; //!< the time since the last update - "lag"
		double m_CurrentTime; //!< current time of update
		double m_PreviousTime; //!< previous time when updated
		glm::mat4 m_setModel;		
		bool m_DrawDebugBool;

		GLuint m_wireVaoHandle;
		GLuint m_wireVboHandle[2];
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
