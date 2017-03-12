#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletDynamics\Vehicle\btRaycastVehicle.h>
#include <BulletDynamics\ConstraintSolver\btHingeConstraint.h>
#include <BulletDynamics\ConstraintSolver\btSliderConstraint.h>

#include "modelLoader.h"
#include "meshLoader.h"
#include "shader.h"

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

	void update(float deltaTime);
	void render(std::shared_ptr<GRAPHICS::Shader> shader);

	btRigidBody* m_carChassis;
	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colShape);

	int m_wheelInstances[4];

	//

private:
	std::shared_ptr<GRAPHICS::Model> m_car = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carRwheels = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carFrontR = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carFrontL = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_map = nullptr;

};