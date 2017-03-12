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

namespace PHYSICS
{
	class PhysicsController {

	public:
		
		static PhysicsController& getPhysicsInstance();

		~PhysicsController();

		void StepSimulation(double deltaTime);

		void AddRigidBody(float mass, const btTransform& startTransform, std::shared_ptr<btCollisionShape> Shape);

		void DrawDebugWorld();

		//static std::shared_ptr<btCollisionShape> CreateStaticCollideShape(std::shared_ptr<GRAPHICS::Mesh> mesh, const btVector3& scale);
		static std::shared_ptr<std::vector<btCollisionShape*>> CreateStaticCollisionShapes(std::shared_ptr<GRAPHICS::Model> model, float scale);
		static std::shared_ptr<std::vector<btCollisionShape*>> CreateStaticCollisionShapes(std::shared_ptr<GRAPHICS::Mesh> model, float scale);

		std::shared_ptr<RaceCar> getCar();

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

		std::shared_ptr<RaceCar> m_raceCar;
		// shared ptr of weapons
		// checkpoints?
		// map points?

		//SINGLETON
		PhysicsController();

		
	};
}