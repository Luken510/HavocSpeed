#include "game.h"

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


GAME::Game::Game() : m_timeSinceLastUpdate(0), m_CurrentTime(0), m_PreviousTime(0) {
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
	//init camera
	m_window.resizeGL(m_camera, 1280, 720);
	gl::Enable(gl::DEPTH_TEST);
	//link shaders
	m_objShader->CompileAndLinkShader("./external/assets/shaders/object.vs", "./external/assets/shaders/object.fs");
	m_MapShader->CompileAndLinkShader("./external/assets/shaders/map.vs", "./external/assets/shaders/map.fs");
	// physics engine
	

	//m_worldPhysics->
	
	EventHandler.setCamera(m_camera);
	glfwSetScrollCallback(m_window.GetWindow(), &EventHandler.ScrollButtonCallBack);
	glfwSetMouseButtonCallback(m_window.GetWindow(), &EventHandler.mouseButtonCallback);
	// need to make a fair bit of get/set because of this work around???
	// load objects, textures
	m_car = std::make_shared<GRAPHICS::Model>("./external/assets/car/CarChasis.obj");
	m_carRwheels = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearWheels.obj");
	m_carFrontR = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontRightWheel.obj");
	m_carFrontL = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontLeftWheel.obj"); // similar to the robot, built car class and init them all in there.
	m_map = std::make_shared<GRAPHICS::Model>("./external/assets/map/track01_.3ds");

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

		while (m_timeSinceLastUpdate >= MS_PER_UPDATE) {
			Update(MS_PER_UPDATE);
			m_timeSinceLastUpdate -= MS_PER_UPDATE;
		}

		Render(m_timeSinceLastUpdate / MS_PER_UPDATE);
	}
}

void GAME::Game::HandleKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mods) // static
{
	
}

void GAME::Game::Update(float deltaTime)
{
	m_window.update(deltaTime, m_camera);
}

void GAME::Game::Render(float Interpolate)
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


	m_objShader->Use();
	//need to create the car class to enable it to move/set its position elsewhere from here.
	m_setModel = glm::mat4(1.0f) * glm::translate(glm::vec3(85.0f, 1.0f, 0.0f)) *glm::scale(glm::vec3(0.05f, 0.05f, 0.05f));
	setmatricies(m_objShader);
	m_car->Render(m_objShader);
	m_carRwheels->Render(m_objShader);
	m_carFrontR->Render(m_objShader);
	m_carFrontL->Render(m_objShader);

	m_MapShader->Use();
	m_setModel = glm::mat4(1.0f) * glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	setmatricies(m_MapShader);
	m_map->Render(m_MapShader);
	
	m_window.Display();
}

void GAME::Game::setmatricies(std::shared_ptr<GRAPHICS::Shader> Shader)
{
	
	
	//move to set up matricies
	Shader->SetUniform("model", m_setModel);
	Shader->SetUniform("view", m_camera->view());
	Shader->SetUniform("projection", m_camera->projection());

}
