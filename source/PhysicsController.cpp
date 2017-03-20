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
	m_dynamicWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

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

PHYSICS::PhysicsController& PHYSICS::PhysicsController::GetPhysicsInstance()
{
	
	static PHYSICS::PhysicsController* instance = new PhysicsController();

	return *instance; // singleton
}

void PHYSICS::PhysicsController::StepSimulation(double deltaTime)
{
	m_dynamicWorld->stepSimulation(deltaTime);
}

void PHYSICS::PhysicsController::AddRigidBody(btRigidBody * body)
{
	
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

btRigidBody * PHYSICS::PhysicsController::CreateRigidbody(btScalar mass, const btTransform & transform, btBoxShape * shape)
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

glm::mat4 PHYSICS::PhysicsController::btTransTo_glmMat4(const btTransform & transform)
{
	btMatrix3x3 m = transform.getBasis();
	btVector3 v = transform.getOrigin();
	
	
	return glm::mat4(m[0].x(), m[1].x(), m[2].x(), 0,
					 m[0].y(), m[1].y(), m[2].y(), 0,
					 m[0].z(), m[1].z(), m[2].z(), 0,
					 v.x(), v.y(), v.z(), 1);
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

void PHYSICS::PhysicsController::AddModel( btCollisionShape * shape)
{
	m_collisionShapes.push_back(shape);
	
}

void PHYSICS::PhysicsController::AddModel(btConvexHullShape * shape)
{
	m_collisionShapes.push_back(shape);
}




/*std::shared_ptr<RaceCar> PHYSICS::PhysicsController::getCar()
{
	return std::shared_ptr<RaceCar>();
}*/

btDynamicsWorld * PHYSICS::PhysicsController::GetDynamicWorld()
{
	return m_dynamicWorld;
}

void PHYSICS::PhysicsController::AddCar()
{
}

void PHYSICS::PhysicsController::addRocket(const btTransform & transform)
{
}
