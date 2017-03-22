#pragma once
// use this to init the world of physics
// use this to add rigid bodies to it etc.
// use this to do the time step etc.
//see example for details
//init(), addRigidbody(), createcollisionshape(){getmeshes()} , getCar, Memory pool for smaller objects
// etc etc etc.

#include <memory>
#include <btBulletDynamicsCommon.h>

#include "meshLoader.h"
#include "modelLoader.h"
#include "raceCar.h"
#include "assimpToBulletObj.h"
#include "physicsDebugDrawer.h"

namespace PHYSICS
{
	class PhysicsController {

	public:
		
		static PhysicsController& GetPhysicsInstance();

		~PhysicsController();

		void StepSimulation(double deltaTime);


		void DrawDebugWorld();

		btConvexHullShape* CreateConvexHull(const GRAPHICS::ObjInstanceVertex&  vertices, int numOfVerts, int Stride, float scale, btScalar mass);
		void AddModel(btCollisionShape* shape);
		void AddModel(btConvexHullShape* shape);
		void AddRigidBody(btRigidBody * body);
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btCollisionShape* shape);
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btConvexHullShape* shape);
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btBoxShape* shape);
		glm::mat4 btTransTo_glmMat4(const btTransform & transform);
	//	std::shared_ptr<RaceCar> getCar();

		btDynamicsWorld* GetDynamicWorld();
		GRAPHICS::PhysicsDebugDrawer* GetDebugDrawer();

		void AddCar();
		btRaycastVehicle* getCar();
		void addRocket(const btTransform & transform);
		// maybe Add Weapon template instead, with it changing aka virtual
		// somehow add a map to find certain points within the dynamic world

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

		
	};
}