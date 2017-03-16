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

namespace PHYSICS
{
	class PhysicsController {

	public:
		
		static PhysicsController& getPhysicsInstance();

		~PhysicsController();

		void StepSimulation(double deltaTime);


		void DrawDebugWorld();

		btConvexHullShape* CreateConvexHull(const GRAPHICS::ObjInstanceVertex&  vertices, int numOfVerts, int Stride, float scale, btScalar mass);
		void AddModel(btCollisionShape* shape, const btTransform &startingPos, btScalar mass);
		void AddModel(btConvexHullShape* shape, const btTransform &startingPos, btScalar mass);
		void AddRigidBody(float mass, const btTransform& transform, btCollisionShape* shape);
		void AddRigidBody(float mass, const btTransform& transform, btConvexHullShape* shape);
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btCollisionShape* shape);
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btConvexHullShape* shape);
	
	//	std::shared_ptr<RaceCar> getCar();

		void AddCar(const btTransform & transform);
		void addRocket(const btTransform & transform);
		// maybe Add Weapon template instead, with it changing aka virtual
		// somehow add a map to find certain points within the dynamic world

	private:

		btCollisionDispatcher* m_dispatcher = nullptr;
		btBroadphaseInterface* m_broadphase = nullptr;
		btSequentialImpulseConstraintSolver* m_constraintSolver = nullptr;
		btDefaultCollisionConfiguration* m_collisionConfig = nullptr;
		btDiscreteDynamicsWorld* m_dynamicWorld = nullptr;

		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
		//std::shared_ptr<RaceCar> m_raceCar = nullptr;
		// shared ptr of weapons
		// checkpoints?
		// map points?

		//SINGLETON
		PhysicsController();

		
	};
}