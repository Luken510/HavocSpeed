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
	// Clean up, LIFO system

	delete m_dynamicWorld;
	delete m_constraintSolver;
	delete m_dispatcher;
	delete m_collisionConfig;
	delete m_broadphase;
}

PHYSICS::PhysicsController& PHYSICS::PhysicsController::getPhysicsInstance()
{
	
	static PHYSICS::PhysicsController* instance = new PhysicsController();

	return *instance; // singleton
}

void PHYSICS::PhysicsController::StepSimulation(double deltaTime)
{
}

void PHYSICS::PhysicsController::AddRigidBody(float mass, const btTransform & startTransform, btCollisionShape* Shape)
{
	btRigidBody* body = CreateRigidbody(mass, startTransform, Shape);

	m_dynamicWorld->addRigidBody(body);
}
void PHYSICS::PhysicsController::AddRigidBody(float mass, const btTransform & startTransform, btConvexHullShape* Shape)
{
	btRigidBody* body = CreateRigidbody(mass, startTransform, Shape);

	m_dynamicWorld->addRigidBody(body);
}

btRigidBody * PHYSICS::PhysicsController::CreateRigidbody(btScalar mass, const btTransform & transform, btCollisionShape * shape)
{
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);
	
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(info);
	//body set contact processing threshold (0);

	return body;
}

btRigidBody * PHYSICS::PhysicsController::CreateRigidbody(btScalar mass, const btTransform & transform, btConvexHullShape * shape)
{
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(info);
	//body set contact processing threshold (0);

	return body;
}

void PHYSICS::PhysicsController::DrawDebugWorld()
{
}

btConvexHullShape*  PHYSICS::PhysicsController::CreateConvexHull(const GRAPHICS::ObjInstanceVertex& vertices, int numOfVerts, 
																	int Stride, float scale, btScalar mass)
{
	int i = Stride;
	btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&(vertices.xyzw[0])), numOfVerts, Stride);
	btVector3 localscaling(scale, scale, scale);
	shape->setLocalScaling(localscaling);
	shape->optimizeConvexHull();

	return shape;
}

void PHYSICS::PhysicsController::AddModel( btCollisionShape * shape, const btTransform& startingPos, btScalar mass)
{
	m_collisionShapes.push_back(shape);

	AddRigidBody(mass, startingPos, shape);
}

void PHYSICS::PhysicsController::AddModel(btConvexHullShape * shape, const btTransform &startingPos, btScalar mass)
{
	m_collisionShapes.push_back(shape);

	AddRigidBody(mass, startingPos, shape);
}


/*std::shared_ptr<RaceCar> PHYSICS::PhysicsController::getCar()
{
	return std::shared_ptr<RaceCar>();
}*/

void PHYSICS::PhysicsController::AddCar(const btTransform & transform)
{
}

void PHYSICS::PhysicsController::addRocket(const btTransform & transform)
{
}
