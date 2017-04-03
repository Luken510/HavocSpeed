#include <btBulletDynamicsCommon.h>
#include <bullet\BulletCollision\CollisionShapes\btConcaveShape.h>
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>

#include "PhysicsController.h"

#include "physicsDebugDrawer.h"

PHYSICS::PhysicsController::PhysicsController()
{
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
	delete m_debugDrawer;
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

void PHYSICS::PhysicsController::AddStaticRigidBody(btCollisionShape* shape, const btTransform & trans, const btVector3 & scale)
{

	btRigidBody* TempRigidBody;

	TempRigidBody = CreateRigidbody(0, trans, shape);

	m_dynamicWorld->addRigidBody(TempRigidBody);

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
	//UTIL::LOG(UTIL::LOG::INFO) << " Size of array sent into convexHull : " << vertices->size();

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

std::vector<btConvexHullShape *> PHYSICS::PhysicsController::CreateConvexHull(std::shared_ptr<GRAPHICS::Model> model, float scale, btScalar mass, bool optimise)
{

	std::vector<btConvexHullShape*> shape;

	for (unsigned int i = 0; i < model->GetMeshes().size(); i++)
	{
		btConvexHullShape* meshShape = new btConvexHullShape((btScalar*)(&model->GetMeshes()[i].m_vertices.at(0)), model->GetMeshes()[i].m_vertices.size(), sizeof(GRAPHICS::Vertex));

		if (meshShape)
			shape.push_back(meshShape);
	}

	for (unsigned int y = 0; y < shape.size(); y++)
	{
		btVector3 localscaling(scale, scale, scale);
		shape[y]->setLocalScaling(localscaling);
		if (optimise)
		{
			shape[y]->optimizeConvexHull();
		}
	}

	return shape;
}

std::vector<btCollisionShape*> PHYSICS::PhysicsController::CreateMultiCollisionShapes(GRAPHICS::Model * model, const btVector3 & scale)
{
	std::vector<btCollisionShape*> ModelShape;

	std::shared_ptr<GRAPHICS::ObjInstanceShape> glmesh = GRAPHICS::AssimpToBulletObj(model->GetMeshes());

		
		btCollisionShape* MeshShape = CreateCollisionShape(glmesh, scale);

		if (MeshShape)
			ModelShape.push_back(MeshShape);


	return ModelShape;
}

std::vector<btCollisionShape*> PHYSICS::PhysicsController::CreateMultiCollisionShapes(std::shared_ptr<GRAPHICS::Model> model, const btVector3 & scale)
{
	
	std::vector<btCollisionShape*> ModelShape;
	
	
	//std::shared_ptr<GRAPHICS::ObjInstanceShape> ModelObj = GRAPHICS::AssimpToBulletObj(model->GetMeshes());

	for (unsigned int i = 0; i < model->GetMeshes().size(); i++)
	{
		btCollisionShape* MeshShape = CreateCollisionShape(model->GetMeshes()[i], scale);

		if (MeshShape)
			ModelShape.push_back(MeshShape);

	}

	return ModelShape;
	
}

btCollisionShape * PHYSICS::PhysicsController::CreateCollisionShape(std::shared_ptr<GRAPHICS::ObjInstanceShape> model, const btVector3 & scale)
{

	int triangles = model->m_numOfIndices;
	int numVertices = model->m_numOfVertices;

	if (triangles < 1 || numVertices < 3)
		return NULL;
	
	
	btIndexedMesh test;
	std::vector<float> xyz;
	xyz.clear();
	std::vector<int> idxCount;
	idxCount.clear();

	for (int i = 0; i < numVertices; i++)
	{
		xyz.push_back((model->m_vertices->at(i).xyz[0]));
		xyz.push_back((model->m_vertices->at(i).xyz[1]));
		xyz.push_back((model->m_vertices->at(i).xyz[2]));
	}

	for (int y = 0; y <triangles; y++)
	{
		idxCount.push_back(model->m_indices->at(y));
	}

	btTriangleIndexVertexArray* triShape = new btTriangleIndexVertexArray();
		test.m_vertexBase = (const unsigned char*)(&xyz.at(0));
		test.m_vertexStride = sizeof(GRAPHICS::ObjInstanceVertex);
		test.m_numVertices = (xyz.size()) - 3;
		test.m_numTriangles = (idxCount.size()) - 3;
		test.m_triangleIndexBase = (const unsigned char*)(&idxCount.at(0));
		test.m_triangleIndexStride = sizeof(int);
		test.m_vertexType = PHY_FLOAT;
		test.m_indexType = PHY_INTEGER;

		//triShape->setScaling(scale);
		triShape->addIndexedMesh(test);

		btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(triShape, true);
		shape->setLocalScaling(scale);
		UTIL::LOG(UTIL::LOG::INFO) << "Created Tri Mesh";

		return shape;
}

btCollisionShape * PHYSICS::PhysicsController::CreateCollisionShape(GRAPHICS::ObjInstanceShape * model, const btVector3 & scale)
{

	btTriangleIndexVertexArray* triShape = new btTriangleIndexVertexArray();
	btIndexedMesh test;

	test.m_vertexBase = (const unsigned char*)(&model->m_vertices->at(0));
	test.m_vertexStride = sizeof(GRAPHICS::ObjInstanceVertex);
	test.m_numVertices = model->m_numOfVertices;
	test.m_numTriangles = model->m_numOfIndices;
	test.m_triangleIndexBase = (const unsigned char*)(&model->m_indices->at(0));
	test.m_triangleIndexStride = sizeof(int);
	test.m_indexType = PHY_INTEGER;

	triShape->addIndexedMesh(test);

	btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(triShape, true);
	shape->setLocalScaling(scale);

	return shape;
}

btCollisionShape * PHYSICS::PhysicsController::CreateCollisionShape(const GRAPHICS::Mesh mesh, const btVector3 & scale)
{
	std::vector<float> xyz;
	
	for (unsigned int i = 0; i < mesh.m_vertices.size(); i++)
	{
		xyz.push_back(mesh.m_vertices.at(i).m_position.x);
		xyz.push_back(mesh.m_vertices.at(i).m_position.y);
		xyz.push_back(mesh.m_vertices.at(i).m_position.z);
	}
	
	
	btTriangleIndexVertexArray* triShape = new btTriangleIndexVertexArray();
	btIndexedMesh idxMesh;
	idxMesh.m_vertexBase = (const unsigned char*)(&xyz.at(0));
	idxMesh.m_vertexStride = sizeof(float);
	idxMesh.m_vertexType = PHY_FLOAT;
	idxMesh.m_numVertices = mesh.m_vertices.size();
	idxMesh.m_numTriangles = mesh.m_indices.size() / 3;
	idxMesh.m_triangleIndexBase = (const unsigned char*)(&mesh.m_indices.at(0));
	idxMesh.m_triangleIndexStride = sizeof(unsigned int) * 3;
	idxMesh.m_indexType = PHY_INTEGER;

	triShape->addIndexedMesh(idxMesh);

	btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(triShape, true);
	shape->setLocalScaling(scale);

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

