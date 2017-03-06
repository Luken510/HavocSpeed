
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#include "game.h"


/*
int main()
{
	std::cout << "Hello World!" << std::endl;
	//build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	//set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);

	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
	
	// the actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// the world
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// do things

	// plane
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	
	//shpere to fall down
	btCollisionShape* fallShape = new btSphereShape(1);
	
	//init the ground
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	// first and last params are mass and inertia, set to 0 as it doesnt move
	// bullet takes 0 to mean infinite - it cant move
	btRigidBody::btRigidBodyConstructionInfo groundrigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody *groundRigidBody = new btRigidBody(groundrigidBodyCI);

	//add the ground to the world
	dynamicsWorld->addRigidBody(groundRigidBody);
	// vector 3 places it 50m above ground
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	
	//mass, inertia - to calc the inertia of a sphere use bullets ulitity functions
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia); // calcs inertia

	//construct rigid body and add to the world
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
	
	/*A quick explanation of btRigidBody::btRigidBodyConstructionInfo is in order; when bodies are constructed,
		they are passed certain parameters.This is done through a special structure Bullet provides 
		for this.The components of the btRigidBodyConstructionInfo are copied into the body when you 
			construct it, and only used at initialisation time.If you want to create a thousand bodies
			with exactly the same properties, you only need to build one btRigidBodyConstructionInfo, 
			and pass that same one to all the bodies that you create.
			

	 IDEA:  Create a vector/array of rigid bodies, every time we add an 
				object we run update/process and it adds this to the array,
				the call an update/ADD function that adds all the current rigidbodyes to the world
				can then call update like bullet needs to run through all the physics???
				TO DO : READ UP ON THE TIME STEP FOR BULLET AND HOW YOU CAN USE IT
	for (int i = 0; i < 300; i++) {

		dynamicsWorld->stepSimulation(1 / 60.f, 10);

		btTransform trans;
		fallRigidBody->getMotionState()->getWorldTransform(trans);

		std::cout << "Sphere Height: " << trans.getOrigin().getY() << std::endl;
	}




	// Clean up, LIFO system
	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	delete fallShape;

	delete groundShape;

	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;


	system("PAUSE");
	return 0;
}*/ 

// take a Screenshot of Debugging/Testing BULLET // SCREEN SHOT FOR EVIDENCE OF DEBUGGING AND TESTING WITH BULLET




int main()
{	
	GAME::Game Game;
	Game.Init();

	return 0;
}
