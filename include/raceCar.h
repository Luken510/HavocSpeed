#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletDynamics\Vehicle\btRaycastVehicle.h>
#include <BulletDynamics\ConstraintSolver\btHingeConstraint.h>
#include <BulletDynamics\ConstraintSolver\btSliderConstraint.h>

#include "modelLoader.h"
#include "meshLoader.h"
#include "PhysicsController.h"
#include "shader.h"

struct VehicleTuning;
struct VehicleRaycaster;
struct CollisionShape;


namespace CarConfig
{
	struct Config
	{
		btScalar mass;
		btScalar dampingLinear;
		btScalar dampingAngle;
	};
}



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

	void init();

	void CreateCarBulletObjFromModel();

	void update(float deltaTime);
	void render(std::shared_ptr<GRAPHICS::Shader> shader);

	
	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colShape);
	//

private:
	std::shared_ptr<GRAPHICS::Model> m_car = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carRwheels = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carFrontR = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carFrontL = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_map = nullptr;

	btConvexHullShape* carChasis = nullptr;

	btVehicleRaycaster* m_carRayCaster = nullptr;
	btRaycastVehicle* m_rayCar = nullptr;
	btRigidBody* m_carChassis = nullptr;
	btCompoundShape* m_carCompound = nullptr;
	btConvexTriangleMeshShape* m_carTriMeshShape = nullptr;
	btBvhTriangleMeshShape* m_carTestTri = nullptr;


	int m_wheelInstances[4];
};