#include "game.h"

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
	//init camera
	m_window.resizeGL(m_camera, 1280, 720);
	gl::Enable(gl::DEPTH_TEST);
	
	//link shaders
	m_objShader->CompileAndLinkShader("./external/assets/shaders/object.vs", "./external/assets/shaders/object.fs");
	m_MapShader->CompileAndLinkShader("./external/assets/shaders/map.vs", "./external/assets/shaders/map.fs");
	m_debugDrawerShader->CompileAndLinkShader("./external/assets/shaders/line.vs", "./external/assets/shaders/line.fs");
	// physics engine
	PHYSICS::PhysicsController::GetPhysicsInstance();
	//Event Handler
	UTIL::EventHandler::getInstance();

	//car
	m_player1Car = std::make_shared<RaceCar>();
	m_player1Car->Init();
	
	//track
	m_map = std::make_shared<GRAPHICS::Model>("./external/assets/map/track/racetrack3d.3ds");

	UTIL::EventHandler::getInstance().setCamera(m_camera);
	UTIL::EventHandler::getInstance().setCar(m_player1Car);
	glfwSetScrollCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().ScrollButtonCallBack);
	glfwSetMouseButtonCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().MouseButtonCallback);
	glfwSetKeyCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().KeyCallBack);

	

	//move to map class??

	btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(500.0), btScalar(10.0), btScalar(500.0)));

	PHYSICS::PhysicsController::GetPhysicsInstance().AddModel(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -10, 0));

	
	PHYSICS::PhysicsController::GetPhysicsInstance().AddRigidBody(PHYSICS::PhysicsController::GetPhysicsInstance().CreateRigidbody(0, groundTransform, groundShape));
	

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
	m_window.update(deltaTime, m_camera);
	m_player1Car->Update(deltaTime);
	PHYSICS::PhysicsController::GetPhysicsInstance().StepSimulation(deltaTime);
	
}

void GAME::Game::Render(double Interpolate)
{
	
	
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	
	//if (m_DrawDebugBool)
	//{
		m_debugDrawerShader->Use();
		m_setModel = glm::mat4(1.0f);
		setmatricies(m_debugDrawerShader);

		PHYSICS::PhysicsController::GetPhysicsInstance().DrawDebugWorld();

		WireFrameMode(PHYSICS::PhysicsController::GetPhysicsInstance().GetDebugDrawer()->GetLines());
	//}
	//else {
		m_objShader->Use();
		//need to create the car class to enable it to move/set its position elsewhere from here.
		m_setModel = glm::mat4(1.0f) * m_player1Car->GetCarMatrix();
		setmatricies(m_objShader);
		m_player1Car->Render(m_objShader);

		m_MapShader->Use();
		m_setModel = glm::mat4(1.0f) * glm::translate(glm::vec3(0.0f, -100.0f, 0.0f)) * glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(0.05f, 0.05f, 0.05f));
		setmatricies(m_MapShader);
		m_map->Render(m_MapShader);
	//}
	

	m_window.Display();

	
}

void GAME::Game::setmatricies(std::shared_ptr<GRAPHICS::Shader> Shader)
{

	Shader->SetUniform("model", m_setModel);
	Shader->SetUniform("view", m_camera->view());
	Shader->SetUniform("projection", m_camera->projection());

}

void GAME::Game::WireFrameMode(std::vector<GRAPHICS::Line> & lines)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
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

	//gl::GenVertexArrays(1, &m_wireVaoHandle);
	gl::BindVertexArray(m_wireVaoHandle);

	gl::GenBuffers(2, m_wireVboHandle);

	gl::BindBuffer(gl::ARRAY_BUFFER, m_wireVboHandle[0]);
	gl::BufferData(gl::ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), (void*)&(vertices.at(0)), gl::STATIC_DRAW);

	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, NULL);


	gl::DrawElements(gl::LINES, indices.size(), gl::UNSIGNED_INT, (void*)&(indices.at(0)));

	PHYSICS::PhysicsController::GetPhysicsInstance().GetDebugDrawer()->ClearLines();
	
}
