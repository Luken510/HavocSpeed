#include "game.h"

#include <GLFW/glfw3.h>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#include "logger.h"
#include "modelLoader.h"


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
	// physics engine
	glfwSetKeyCallback((m_window.GetWindow()),HandleEvents);

	//https://msdn.microsoft.com/en-us/library/hh279669.aspx
	// Car = std::make_shared<GRAPHICS::Model>("../external/assets/Car/Futuristic_Car_2.1_blend.blend");

	/*//build the broadphase
	broadphase = new btDbvtBroadphase();

	//set up the collision configuration and dispatcher
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);

	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

	// the actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// the world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	*/

	// load objects, textures



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

void GAME::Game::HandleEvents(GLFWwindow* window, int key, int scancode, int action, int mods) // static
{

}

void GAME::Game::Update(float deltaTime)
{

}

void GAME::Game::Render(float Interpolate)
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	m_window.Display();
}
