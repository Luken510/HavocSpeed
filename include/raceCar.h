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
		btScalar m_mass, //!< mass of the car
			m_dampingLinear,  //!< Linear Damping of the chasis
			m_dampingAngle,  //!< Angular Damping of the chasis
			m_maxEnginePower,  //!< Max amount of torque applied to the wheels
			m_maxBreakingPower, //!< Max amount of breaking torque applied to the wheels
			m_maxSpeed, //!< max speed the car can achieve (km/h) - km cause of bullet
			m_turningIncrement, //!< how fast the car turns
			m_turningDecrement, //!< how fast the car resets
			m_turningLimits, //!< max angle the car can turn
			m_wheelFriction,  //!< friction between the tyre and ground
			m_wheelRadius,  //!< wheel radius
			m_wheelWidth, //!< wheel width
			m_wheelConnectionHeight,  //!< starting point of the ray in chasis space (y)
			m_wheelConnectionWidth,  //!< starting point of the ray in chasis space (x)
			m_wheelConnectionLength, //!< starting point of the ray in chasis space (z)
			m_suspensionStiffness, //!< stiffness constant for suspension. 10 - off road, 50 - sports car, 200 - f1 car
			m_suspensionDampRelaxtion, //!< damping coefficient for when the suspension is expanding
			m_suspensionDampCompression,//!< damping coefficient for when the suspension is compressed
			m_suspensionMaxLength, //!< the max length of the suspension (metres)
			m_suspensionMaxTravel, //!< the maximum distance the suspension can be compressed
			m_rollInfluence; //!< reduces the rolling torque applied from the wheel that cause the vehicle to roll over - a slight hack

		btVector3 m_wheelDirection, //!< the direction of the ray cast in chasis space
				  wheelAxel; //!< the axis that the wheels rotate around

		
		Config() :
			m_mass(5.0f),
			m_dampingLinear(0.25f),
			m_dampingAngle(0.0f),
			m_maxEnginePower(22.5f),
			m_maxBreakingPower(0.5f),
			m_maxSpeed(60.0f),
			m_turningIncrement(0.025f),
			m_turningDecrement(0.1f),
			m_turningLimits(0.50f),
			m_wheelFriction(2.5f),
			m_wheelRadius(0.302f),
			m_wheelWidth(0.215f),
			m_wheelConnectionHeight(0.200f),
			m_wheelConnectionWidth(0.725f),
			m_wheelConnectionLength(1.325f),
			m_suspensionStiffness(160.f),
			m_suspensionDampRelaxtion(11.7f),
			m_suspensionDampCompression(10.7f),
			m_suspensionMaxLength(0.06f),
			m_suspensionMaxTravel(12.0f),
			m_rollInfluence(0.0f),
			m_wheelDirection(0.0f, -1.0f, -0.0f),
			wheelAxel(-1.0f, -0.0f, 0.0f)
		{
		}
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

	void Init();

	void CreateCarBulletObjFromModel();

	void Update(double deltaTime);
	void Render(std::shared_ptr<GRAPHICS::Shader> shader);

	void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale);
	void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle);
	void UpdateMatrix(glm::mat4 matrix);
	glm::mat4 GetCarMatrix();
	glm::mat4 getWorldPos();

	void Drive();
	void Reverse();
	void TurnLeft();
	void TurnRight();
	void Brake();
	
	//btRigidBody* LocalCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colShape);
	//

private:
	std::shared_ptr<GRAPHICS::Model> m_car = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carFrontR = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carFrontL = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carRearR = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carRearL = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_map = nullptr;

	CarConfig::Config carConfig;

	btConvexHullShape* m_carChasis = nullptr;
	btCompoundShape* m_carChasisComp = nullptr;
	btVehicleRaycaster* m_carRayCaster = nullptr;
	btRaycastVehicle* m_raycastCar = nullptr;
	btRigidBody* m_carChassisRigid = nullptr;

	float m_enginePower;
	float m_brakingPower;
	float m_steeringPower;

	glm::mat4 m_carModelMatrix;
	glm::vec3 m_carScale = glm::vec3(0.08f, 0.08f, 0.08f);

	int m_wheelInstances[4];
};