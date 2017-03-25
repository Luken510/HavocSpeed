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
																	int Stride, float scale, btScalar mass, bool optimise)
{
	UTIL::LOG(UTIL::LOG::INFO) << " Size of array sent into convexHull : " << vertices->size();

	btConvexHullShape* shape = new btConvexHullShape();

	///It is easier to not pass any points in the constructor, and just add one point at a time, using addPoint.

	for (int i = 0; i < (int)vertices->size(); i++)
	{
		btVector3 a = btVector3(vertices->at(i).xyz[0], vertices->at(i).xyz[1], vertices->at(i).xyz[2]);
		shape->addPoint(a);
	}


	btVector3 localscaling(scale, scale, scale);
	shape->setLocalScaling(localscaling);

	if (optimise)
	{
		shape->optimizeConvexHull();
	}

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


btCollisionShape * PHYSICS::PhysicsController::CreateCollisionShape(btAlignedObjectArray<GRAPHICS::ObjInstanceVertex>* vertices, int numOfVerts, int vertStride, float scale,
	btScalar mass, btAlignedObjectArray<int>* indices, int numOfIndices, int indicesStride)
{

	btTriangleIndexVertexArray* triShape = new btTriangleIndexVertexArray(numOfIndices, (int*)&(indices->at(0)), indicesStride,
		numOfVerts*3, (btScalar*)&(vertices->at(0).xyz[0]), vertStride);

	btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(triShape, true);
	shape->setLocalScaling(btVector3(scale, scale, scale));

	return shape;
}