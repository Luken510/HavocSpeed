#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "meshLoader.h"
#include "modelLoader.h"
#include "assimpToBulletObj.h"
#include "physicsDebugDrawer.h"

class RaceCar;

namespace PHYSICS
{
	class PhysicsController {

	public:
		
		static PhysicsController& GetPhysicsInstance();

		~PhysicsController();

		void StepSimulation(double deltaTime);


		void DrawDebugWorld();
	
		btConvexHullShape* CreateConvexHull(btAlignedObjectArray<GRAPHICS::ObjInstanceVertex>* vertices, int numOfVerts, int Stride, float scale, btScalar mass, bool optimise);
		std::vector<btConvexHullShape*> CreateConvexHull(std::shared_ptr<GRAPHICS::Model> model, float scale, btScalar mass, bool optimise);
		std::vector<btCollisionShape*> CreateMultiCollisionShapes(GRAPHICS::Model* model, const btVector3 &scale);
		std::vector<btCollisionShape*> CreateMultiCollisionShapes(std::shared_ptr<GRAPHICS::Model> model, const btVector3 &scale);
		btCollisionShape* CreateCollisionShape(std::shared_ptr<GRAPHICS::ObjInstanceShape> model, const btVector3 & scale);
		btCollisionShape* CreateCollisionShape(GRAPHICS::ObjInstanceShape * model, const btVector3 & scale);
		btCollisionShape* CreateCollisionShape( const GRAPHICS::Mesh mesh, const btVector3 & scale);


		void AddModel(btCollisionShape* shape);
		void AddModel(btConvexHullShape* shape);

		void AddRigidBody(btRigidBody * body);
		void AddStaticRigidBody(btCollisionShape* shapes, const btTransform& trans, const btVector3 & scale);

		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btCollisionShape* shape);
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btConvexHullShape* shape);
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btBoxShape* shape);
	//	std::shared_ptr<RaceCar> getCar();

		btDynamicsWorld* GetDynamicWorld();
		GRAPHICS::PhysicsDebugDrawer* GetDebugDrawer();

		void AddCar();
		btRaycastVehicle* getCar();
		void addRocket(const btTransform & transform);
		// maybe Add Weapon template instead, with it changing aka virtual
		// somehow add a map to find certain points within the dynamic world
		glm::mat4 btTransToGlmMat4(const btTransform & transform);

	private:

		btCollisionDispatcher* m_dispatcher = nullptr;
		btBroadphaseInterface* m_broadphase = nullptr;
		btSequentialImpulseConstraintSolver* m_constraintSolver = nullptr;
		btDefaultCollisionConfiguration* m_collisionConfig = nullptr;
		btDiscreteDynamicsWorld* m_dynamicWorld = nullptr;
		GRAPHICS::PhysicsDebugDrawer* m_debugDrawer = nullptr;

		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
		//std::shared_ptr<RaceCar> m_raceCar = nullptr;
		// shared ptr of weapons
		// checkpoints?
		// map points?

		//SINGLETON
		PhysicsController();
		PhysicsController(PhysicsController const&); //prevents copies
		void operator=(PhysicsController const&); //prevents assignments

		
	};
}