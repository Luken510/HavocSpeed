#include "game.h"

#include <iostream>
#include <gl.hpp>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "logger.h"
#include "shader.h"
#include "modelLoader.h"
#include "PhysicsController.h"
#include "QuatCamera.h"
#include "eventHandler.h"


GAME::Game::Game() : m_timeSinceLastUpdate(0), m_CurrentTime(0), m_PreviousTime(0), m_DrawDebugBool(false) {
	UTIL::LOG::Clear();
}


GAME::Game::~Game()
{
	// Clean up, LIFO system
	//dynamicsWorld->removeRigidBody(fallRigidBody);
	//delete fallRigidBody->getMotionState();
	//delete fallRigidBody;

	//delete dynamicsWorld;
	//delete solver;
	//delete dispatcher;
	//delete collisionConfig;
	//delete broadphase;

}

void GAME::Game::Init()
{
	m_CameraController = m_freeRoamCamera;
	//init camera
	m_window.resizeGL(m_CameraController, 1280, 720);
	gl::Enable(gl::DEPTH_TEST);
	
	//link shaders
	m_objShader->CompileAndLinkShader("./external/assets/shaders/object.vs", "./external/assets/shaders/object.fs");
	m_MapShader->CompileAndLinkShader("./external/assets/shaders/map.vs", "./external/assets/shaders/map.fs");
	m_debugDrawerShader->CompileAndLinkShader("./external/assets/shaders/line.vs", "./external/assets/shaders/line.fs");
	// physics engine
	PHYSICS::PhysicsController::GetPhysicsInstance();
	//Event Handler
	UTIL::EventHandler::getInstance();

	//set camera
	CurrentCamera();
	//car
	m_player1Car = std::make_shared<RaceCar>();
	m_player1Car->Init();
	//
	m_freeRoamCamera->Follow(m_player1Car->GetCarMatrix(), 90.0f);
	
	//track
	m_map = std::make_shared<Map>();
	m_map->Init();

	UTIL::EventHandler::getInstance().setCamera(m_CameraController);
	UTIL::EventHandler::getInstance().setCar(m_player1Car);
	glfwSetScrollCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().ScrollButtonCallBack);
	glfwSetMouseButtonCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().MouseButtonCallback);
	glfwSetKeyCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().KeyCallBack);
	RunGame();

}

void GAME::Game::RunGame()
{
	m_PreviousTime = glfwGetTime();
	//main game loop

	while (!glfwWindowShouldClose(m_window.GetWindow()) && !glfwGetKey(m_window.GetWindow(), GLFW_KEY_ESCAPE)) 
	{

		m_CurrentTime = (double)glfwGetTime();
		double elaspedTime = m_CurrentTime - m_PreviousTime; //m_CurrentTime - m_PreviousTime;
		m_PreviousTime = m_CurrentTime;
		m_timeSinceLastUpdate += elaspedTime;
	
		glfwPollEvents();
		PollKeyEvents();
		CurrentCamera();
		
		m_DrawDebugBool = UTIL::EventHandler::getInstance().getWireBool();

		while (m_timeSinceLastUpdate >= MS_PER_UPDATE) {
			Update(MS_PER_UPDATE);
			m_timeSinceLastUpdate -= MS_PER_UPDATE;
		}

		Render(m_timeSinceLastUpdate / MS_PER_UPDATE);
		
	}
}


void GAME::Game::Update(double deltaTime)
{
	m_window.update(deltaTime, m_CameraController);
	m_player1Car->Update(deltaTime);
	m_map->Update(deltaTime);
	m_CameraController->update(m_player1Car->GetCarMatrix());
	

	PHYSICS::PhysicsController::GetPhysicsInstance().StepSimulation(deltaTime);
	
}

void GAME::Game::Render(double Interpolate)
{
	
	
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	
	if (m_DrawDebugBool)
	{
		m_debugDrawerShader->Use();
		m_setModel = glm::mat4(1.0f);
		SetViewMatricies(m_debugDrawerShader, m_setModel);

		PHYSICS::PhysicsController::GetPhysicsInstance().DrawDebugWorld();

		WireFrameMode(PHYSICS::PhysicsController::GetPhysicsInstance().GetDebugDrawer()->GetLines());
	}
	else {
		m_objShader->Use();
		m_setModel = glm::mat4(1.0f) * m_player1Car->GetCarMatrix();
		SetViewMatricies(m_objShader, m_setModel);
		m_player1Car->Render(m_objShader);

		m_MapShader->Use();
		m_setModel = glm::mat4(1.0f) * m_map->GetTrackMatrix();
		SetViewMatricies(m_MapShader, m_setModel);
		m_map->Render(m_MapShader);
	}
	

	m_window.Display();

	
}

void GAME::Game::SetViewMatricies(std::shared_ptr<GRAPHICS::Shader> Shader, glm::mat4 model)
{

	Shader->SetUniform("model", model);
	Shader->SetUniform("view", m_CameraController->view());
	Shader->SetUniform("projection", m_CameraController->projection());

}

void GAME::Game::WireFrameMode(std::vector<GRAPHICS::Line> & lines)
{
	std::vector<GLfloat> vertices;
	//vertices.reserve(100000);
	std::vector<GLuint> indices;
	//indices.reserve(100000);
	unsigned int index = 0;


	for (std::vector<GRAPHICS::Line>::iterator it = lines.begin(); it != lines.end(); it++) {

		GRAPHICS::Line temp = *it;
		//start line
		vertices.push_back(temp.start.x);
		vertices.push_back(temp.start.y);
		vertices.push_back(temp.start.z);
		//end of line
		vertices.push_back(temp.finish.x);
		vertices.push_back(temp.finish.y);
		vertices.push_back(temp.finish.z);

		//index
		indices.push_back(index);
		indices.push_back(index + 1);
		index += 2;
	}

	UTIL::LOG(UTIL::LOG::INFO) << "Number of Vertices being draw : " << vertices.size();
	UTIL::LOG(UTIL::LOG::INFO) << " ";
	UTIL::LOG(UTIL::LOG::INFO) << "Number of Indices being draw : " << vertices.size();
	UTIL::LOG(UTIL::LOG::INFO) << " ";
	UTIL::LOG(UTIL::LOG::INFO) << "Size of Debug Lines :" << lines.size();

	gl::BindVertexArray(m_wireVaoHandle);

	gl::GenBuffers(2, m_wireVboHandle);

	gl::BindBuffer(gl::ARRAY_BUFFER, m_wireVboHandle[0]);
	gl::BufferData(gl::ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), (void*)&(vertices.at(0)), gl::STATIC_DRAW);

	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, NULL);


	gl::DrawElements(gl::LINES, indices.size(), gl::UNSIGNED_INT, (void*)&(indices.at(0)));

	lines.clear();
	vertices.clear();
	indices.clear();
	PHYSICS::PhysicsController::GetPhysicsInstance().GetDebugDrawer()->ClearLines();
	
}

void GAME::Game::PollKeyEvents()
{
	UTIL::EventHandler::getInstance().PollKeyEvents(m_window.GetWindow());
}

void GAME::Game::CurrentCamera()
{
	m_cameraStates = static_cast<CameraStates>(UTIL::EventHandler::getInstance().getCameraState());
	
	switch (m_cameraStates)
	{
	case STANDARD_VIEW:
		m_CameraController = m_standardCamera;
	//	UTIL::LOG(UTIL::LOG::INFO) << " Camera = Standard Camera";
		break;
	case FIRST_PERSON_VIEW:
		m_CameraController = m_firstPersonCamera;
	//	UTIL::LOG(UTIL::LOG::INFO) << " Camera = FirstPerson Camera";
		break;
	case REAR_VIEW:
		m_CameraController = m_rearViewCamera;
	//	UTIL::LOG(UTIL::LOG::INFO) << " Camera = RearView Camera";
		break;
	case FREE_ROAM:
		m_CameraController = m_freeRoamCamera;	
	//	UTIL::LOG(UTIL::LOG::INFO) << " Camera = FreeRoam Camera";
		break;
	default:
		break;
	}
	
	UTIL::EventHandler::getInstance().setCamera(m_CameraController);
}
