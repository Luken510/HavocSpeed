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
	gl::Enable(gl::DEPTH_TEST);

	carShader.CompileAndLinkShader("./external/assets/car/shaders/car.vs", "./external/assets/car/shaders/car.fs");
		
	// physics engine
	EventHandler.setCamera(std::make_shared<UTIL::QuatCamera>(Camera));

	glfwSetScrollCallback(m_window.GetWindow(),&EventHandler.ScrollButtonCallBack);
	// need to make a fair bit of get/set because of this work around???
	// load objects, textures

	worldPhysics = std::make_shared<PHYSICS::PhysicsController>(worldPhysics->getPhysicsInstance()); // CHECK IF THIS EVEN WORKS
	car = std::make_shared<GRAPHICS::Model>("./external/assets/car/LEGO_CAR_B1.obj");
	map = std::make_shared<GRAPHICS::Model>("./external/assets/map/Skyscraper+Bay.stl");

	RunGame();

}

void GAME::Game::RunGame()
{
	m_PreviousTime = glfwGetTime();
	//main game loop

	while (!glfwWindowShouldClose(m_window.GetWindow()) && !glfwGetKey(m_window.GetWindow(), GLFW_KEY_ESCAPE)) 
	{

		m_CurrentTime = (double)glfwGetTime();
		double elaspedTime = m_CurrentTime - m_PreviousTime;
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
	
}

void GAME::Game::Render(float Interpolate)
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);


	carShader.Use();
	setLightParams(); // to be replaced
	car->Render(std::make_shared<GRAPHICS::Shader>(carShader));
	map->Render(std::make_shared<GRAPHICS::Shader>(carShader));

	m_window.Display();
}

void GAME::Game::setLightParams()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = model * glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	
	//move to set up matricies
	carShader.SetUniform("model", model);
	carShader.SetUniform("view", Camera.view());
	carShader.SetUniform("projection", Camera.projection());

}
