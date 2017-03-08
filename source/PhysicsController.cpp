#include <btBulletDynamicsCommon.h>
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>

#include "PhysicsController.h"

PHYSICS::PhysicsController::PhysicsController()
{
	std::cout << "Hello World!" << std::endl;
	//build the broadphase
	m_broadphase =	new btDbvtBroadphase(); // make sure way of init works.

	//set up the collision configuration and dispatcher
	m_collisionConfig = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfig);

	btGImpactCollisionAlgorithm::registerAlgorithm(m_dispatcher);

	// the actual physics solver
	m_constraintSolver = new btSequentialImpulseConstraintSolver();

	// the world
	m_dynamicWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_constraintSolver, m_collisionConfig);
	m_dynamicWorld->setGravity(btVector3(0, -9.8, 0));

}

PHYSICS::PhysicsController::~PhysicsController()
{
}

PHYSICS::PhysicsController& PHYSICS::PhysicsController::getPhysicsInstance()
{
	
	static PHYSICS::PhysicsController* instance = new PhysicsController();

	return *instance; // singleton
}

void PHYSICS::PhysicsController::StepSimulation(double deltaTime)
{
}

void PHYSICS::PhysicsController::AddRigidBody(float mass, const btTransform & startTransform, std::shared_ptr<btCollisionShape> Shape)
{
}

void PHYSICS::PhysicsController::DrawDebugWorld()
{
}

std::shared_ptr<std::vector<btCollisionShape*>> PHYSICS::PhysicsController::CreateStaticCollisionShapes(std::shared_ptr<GRAPHICS::Model> model, float scale)
{
	return std::shared_ptr<std::vector<btCollisionShape*>>();
}

std::shared_ptr<RaceCar> PHYSICS::PhysicsController::getCar()
{
	return std::shared_ptr<RaceCar>();
}

void PHYSICS::PhysicsController::AddCar(const btTransform & transform)
{
}

void PHYSICS::PhysicsController::addRocket(const btTransform & transform)
{
}
