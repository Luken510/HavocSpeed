#pragma once

#include <memory>

#include "window.h"
#include "modelLoader.h"
#include "PhysicsController.h"
#include "cameraBase.h"
#include "rearCamera.h"
#include "firstPersonCamera.h"
#include "basicCamera.h"
#include "QuatCamera.h"
#include "eventHandler.h"
#include "raceCar.h"
#include "map.h"
#include "particlePool.h"
#include "particle.h"


#define GLM_FORCE_RADIANS
#define MS_PER_UPDATE (float)0.0166666666666667
#define CAR_TURN_SPEED 0.025f
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

enum GameStates { MAIN_MENU = 0, NORMAL = 1, WITH_PATTERNS = 2, WITHOUT_PATTERNS = 3, TWO_PLAYERS = 4, LOADING = 5 };
enum CameraStates{ STANDARD_VIEW = 0, FIRST_PERSON_VIEW = 1, REAR_VIEW = 2, FREE_ROAM = 3};

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
		void SetViewMatricies(std::shared_ptr<GRAPHICS::Shader> Shader, glm::mat4 model);
		void WireFrameMode(std::vector<GRAPHICS::Line> & lines);
		void PollKeyEvents();
		void CurrentCamera();

		void MainMenu(bool loaded);
		void RenderMainMenu();

		void MemoryPoolTest();
	private:
		GAME::Window m_window; //!< Window Object to render the game		
		std::shared_ptr<GRAPHICS::Shader> m_menuShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<GRAPHICS::Shader> m_objShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<GRAPHICS::Shader> m_MapShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<GRAPHICS::Shader> m_debugDrawerShader = std::make_shared<GRAPHICS::Shader>();
		std::shared_ptr<UTIL::CAMERA::QuatCamera> m_freeRoamCamera = std::make_shared<UTIL::CAMERA::QuatCamera>(glm::vec3(0.0f, 30.0f, 100.0f));
		std::shared_ptr<UTIL::CAMERA::CameraBase> m_standardCamera = std::make_shared<UTIL::CAMERA::BasicCamera>((float)WIN_WIDTH,(float)WIN_HEIGHT /2);
		std::shared_ptr<UTIL::CAMERA::BasicCamera> m_standardCamera2 = std::make_shared<UTIL::CAMERA::BasicCamera>((float)WIN_WIDTH, (float)WIN_HEIGHT /2);
		std::shared_ptr<UTIL::CAMERA::CameraBase> m_rearViewCamera = std::make_shared<UTIL::CAMERA::RearCamera>((float)WIN_WIDTH, (float)WIN_HEIGHT /2);
		std::shared_ptr<UTIL::CAMERA::CameraBase> m_firstPersonCamera = std::make_shared<UTIL::CAMERA::firstPersonCamera>((float)WIN_WIDTH, (float)WIN_HEIGHT);
		std::shared_ptr<UTIL::CAMERA::CameraBase> m_CameraController;
		std::shared_ptr<GAME::Map> m_map = nullptr;
		std::shared_ptr<RaceCar> m_player1Car = nullptr; 
		std::shared_ptr<RaceCar> m_player2Car = nullptr;
		double m_timeSinceLastUpdate; //!< the time since the last update - "lag"
		double m_CurrentTime; //!< current time of update
		double m_PreviousTime; //!< previous time when updated
		glm::mat4 m_setModel;		
		bool m_DrawDebugBool;
		CameraStates m_cameraStates;
		GLuint m_wireVaoHandle;	
		GLuint m_wireVboHandle[2];
		UTIL::ParticlePool m_particleMemoryManager;
		//menu
		GLuint m_menuTexture;
		GLuint m_quad;

	};
}
