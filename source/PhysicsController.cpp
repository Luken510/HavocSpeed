#include <btBulletDynamicsCommon.h>
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>

#include "PhysicsController.h"

#include "physicsDebugDrawer.h"

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

	m_debugDrawer = new GRAPHICS::PhysicsDebugDrawer();
	m_dynamicWorld->setDebugDrawer(m_debugDrawer);

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


void PHYSICS::PhysicsController::DrawDebugWorld()
{
	m_dynamicWorld->debugDrawWorld();
}

btConvexHullShape*  PHYSICS::PhysicsController::CreateConvexHull(btAlignedObjectArray<GRAPHICS::ObjInstanceVertex>* vertices, int numOfVerts, 
																	int Stride, float scale, btScalar mass)
{
	int i = Stride;
	btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&(vertices->at(0).xyzw[0])), numOfVerts, Stride);
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

GRAPHICS::PhysicsDebugDrawer* PHYSICS::PhysicsController::GetDebugDrawer()
{
	return m_debugDrawer;
}

void PHYSICS::PhysicsController::AddCar()
{
}

void PHYSICS::PhysicsController::addRocket(const btTransform & transform)
{
}

glm::mat4 PHYSICS::PhysicsController::btTransToGlmMat4(const btTransform & transform)
{
	//rotation
	btMatrix3x3 rotate = transform.getBasis();
	//position
	btVector3 translate = transform.getOrigin();
	return glm::mat4( rotate[0].x(), rotate[1].x(), rotate[2].x(), 0,
					  rotate[0].y(), rotate[1].y(), rotate[2].y(), 0,
					  rotate[0].z(), rotate[1].z(), rotate[2].z(), 0,
					  translate.x(), translate.y(), translate.z(), 1);
}
