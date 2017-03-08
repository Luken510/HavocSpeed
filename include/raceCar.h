#pragma once

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletDynamics\Vehicle\btRaycastVehicle.h>
#include <BulletDynamics\ConstraintSolver\btHingeConstraint.h>
#include <BulletDynamics\ConstraintSolver\btSliderConstraint.h>


struct VehicleTuning;
struct VehicleRaycaster;
struct CollisionShape;


class RaceCar {

public:
	/*
	\brief Constructor
	*/
	RaceCar();
	/*
	\brief Destructor
	*/
	~RaceCar();

	btRigidBody* m_carChassis;
	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colShape);

	int m_wheelInstances[4];

	//


};