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
		/*! 
		\brief GetPhysicsInstance returns the instance to be used - Singelton.
		*/
		static PhysicsController& GetPhysicsInstance();
		/*! 
		\brief Destructor.
		*/
		~PhysicsController();

		/*! 
		\brief SetpSimulation steps the physics world into an update.
		\param deltaTime the current time within the frame.
		*/
		void StepSimulation(double deltaTime);

		/*! 
		\brief DrawDebugWorld draw's the lines for wireframeMode.
		*/
		void DrawDebugWorld();

		/*! 
		\brief CreateConvexHull Created a convexHull to be used by bullet
		\param vertices vertices deduced from the AssimpToBullet function.
		\param numofVerts number of vertices expected.
		\param Stride how far to step into memory each time
		\param scale the scale of the model
		\param mass the mass of the Bullet object being created
		\param optimise a boolean deciding whether or not to 'optimise' the ConvexHull.
		*/
		btConvexHullShape* CreateConvexHull(btAlignedObjectArray<GRAPHICS::ObjInstanceVertex>* vertices, int numOfVerts, int Stride, float scale, btScalar mass, bool optimise);
		/*!
		\brief CreateConvexHull Created a convexHull to be used by bullet
		\param model Assimp loaded model to be converted.
		\param numofVerts number of vertices expected.
		\param Stride how far to step into memory each time
		\param scale the scale of the model
		\param mass the mass of the Bullet object being created
		\param optimise a boolean deciding whether or not to 'optimise' the ConvexHull.
		*/
		std::vector<btConvexHullShape*> CreateConvexHull(std::shared_ptr<GRAPHICS::Model> model, float scale, btScalar mass, bool optimise);

		/*!
		\brief CreateMultiCollisionShapes Create multiple collision shapes
		\param model Assimp model to be used.
		\param scale the scale of the model
		*/
		std::vector<btCollisionShape*> CreateMultiCollisionShapes(GRAPHICS::Model* model, const btVector3 &scale);

		/*!
		\brief CreateMultiCollisionShapes Create multiple collision shapes
		\param model Shared_ptr to Assimp model to be used.
		\param scale the scale of the model
		*/
		std::vector<btCollisionShape*> CreateMultiCollisionShapes(std::shared_ptr<GRAPHICS::Model> model, const btVector3 &scale);
		/*!
		\brief CreateCollisionShape Create CollisionShape
		\param model shared_ptr to Assimp model to be used.
		\param scale the scale of the model
		*/
		btCollisionShape* CreateCollisionShape(std::shared_ptr<GRAPHICS::ObjInstanceShape> model, const btVector3 & scale);
		/*!
		\brief CreateCollisionShape Create CollisionShape
		\param model Assimp model to be used.
		\param scale the scale of the model
		*/
		btCollisionShape* CreateCollisionShape(GRAPHICS::ObjInstanceShape * model, const btVector3 & scale);
		/*!
		\brief CreateCollisionShape Create CollisionShape
		\param mesh Assimp mesh to be used.
		\param scale the scale of the model
		*/
		btCollisionShape* CreateCollisionShape( const GRAPHICS::Mesh mesh, const btVector3 & scale);


		/*!
		\brief AddModel AddModel
		\param shape shape to be added to bullet physics world.
		*/
		void AddModel(btCollisionShape* shape);
		/*!
		\brief AddModel AddModel
		\param shape ConvexHull shape to be added to bullet physics world.
		*/
		void AddModel(btConvexHullShape* shape);
		/*!
		\brief AddRigidBody AddRigidBody to the physics world
		\param body body to be added to bullet physics world.
		*/
		void AddRigidBody(btRigidBody * body);
		/*!
		\brief AddStaticRigidBody AddStaticRigidBody to the physics world
		\param shapes shape to be added to bullet physics world.
		\param trans the transformation to the model being rendered
		\param scale of the added static rigid body
		*/
		void AddStaticRigidBody(btCollisionShape* shapes, const btTransform& trans, const btVector3 & scale);

		/*!
		\brief CreateRigidbody CreateRigidbody 
		\param mass shape to be added to bullet physics world.
		\param transform the transformation to the model being rendered
		\param shape of the added static rigid body - collision shape
		*/
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btCollisionShape* shape);
		/*!
		\brief CreateRigidbody CreateRigidbody
		\param mass shape to be added to bullet physics world.
		\param transform the transformation to the model being rendered
		\param shape of the added static rigid body - btConvexHullShape shape
		*/
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btConvexHullShape* shape);
		/*!
		\brief CreateRigidbody CreateRigidbody
		\param mass shape to be added to bullet physics world.
		\param transform the transformation to the model being rendered
		\param shape of the added static rigid body - btBoxShape shape
		*/
		btRigidBody* CreateRigidbody(btScalar mass, const btTransform & transform, btBoxShape* shape);

		/*!
		\brief GetDynamicWorld returns the current physics world
		*/
		btDynamicsWorld* GetDynamicWorld();
		/*!
		\brief GetDebugDrawer returns the current Debug drawer 
		*/
		GRAPHICS::PhysicsDebugDrawer* GetDebugDrawer();

		//empty atm
		void addRocket(const btTransform & transform);
		// maybe Add Weapon template instead, with it changing aka virtual
		// somehow add a map to find certain points within the dynamic world

		/*!
		\brief btTransToGlmMat4 function to translate bullet's co-ords to OpenGLs co-ords
		\param transform the variable to be adapted.
		*/
		glm::mat4 btTransToGlmMat4(const btTransform & transform);

	private:

		btCollisionDispatcher* m_dispatcher = nullptr; //!< collision dispatcher
		btBroadphaseInterface* m_broadphase = nullptr; //!< btBroadphaseInterface
		btSequentialImpulseConstraintSolver* m_constraintSolver = nullptr; //!< btSequentialImpulseConstraintSolver
		btDefaultCollisionConfiguration* m_collisionConfig = nullptr; //!< btDefaultCollisionConfiguration
		btDiscreteDynamicsWorld* m_dynamicWorld = nullptr; //!< btDiscreteDynamicsWorld
		GRAPHICS::PhysicsDebugDrawer* m_debugDrawer = nullptr; //!< PhysicsDebugDrawer

		
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes; //!< vector of colliable shapes
	
		//SINGLETON
		/*!
		\brief Constructor - Singleton
		*/
		PhysicsController();
		/*!
		\brief prevents copies
		*/
		PhysicsController(PhysicsController const&); 
		/*!
		\brief prevents assignments
		*/
		void operator=(PhysicsController const&); 

		
	};
}