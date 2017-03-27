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

#define _USE_MATH_DEFINES

#define CAR_SCALE 0.15f


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
			m_rollInfluence, //!< reduces the rolling torque applied from the wheel that cause the vehicle to roll over - a slight hack
			m_bodyToChasisHeight;

		btVector3 m_wheelDirection, //!< the direction of the ray cast in chasis space
				  wheelAxel; //!< the axis that the wheels rotate around
		////////////////////////////////////
		/////////////// TO DO :maybe the model wheels isn#t aligned to the origin, hence the displacement
		///////////////////////: FIGURE OUT HOW TO DO TRIANGLE MESH INSTANCE TO BETTER IMPROVE THE TRACK.
		/////////////////////
		Config() :
			m_mass(5.0f),
			m_dampingLinear(0.25f),
			m_dampingAngle(0.0f),
			m_maxEnginePower(-22.5f), //direction of movement
			m_maxBreakingPower(0.5f),
			m_maxSpeed(60.0f),
			m_turningIncrement(0.025f),
			m_turningDecrement(0.1f),
			m_turningLimits(0.50f),
			m_wheelFriction(2.5f),
			m_wheelRadius(3.302f),
			m_wheelWidth(2.215f),
			m_wheelConnectionHeight(3.9f), //3
			m_wheelConnectionWidth(6.200f), //6
			m_wheelConnectionLength(12.725f), //5.325
			m_suspensionStiffness(160.f),
			m_suspensionDampRelaxtion(12.7f),
			m_suspensionDampCompression(10.7f),
			m_suspensionMaxLength(0.06f),
			m_suspensionMaxTravel(12.0f),
			m_rollInfluence(0.2f),
			m_wheelDirection(0.0f, -1.0f, 0.0f),
			wheelAxel(-1.0f, 0.0f, 0.0f),
			m_bodyToChasisHeight(0.6f)
		{
			m_wheelRadius *= CAR_SCALE;
			m_wheelWidth *= CAR_SCALE;
			m_wheelConnectionHeight *= CAR_SCALE;  
			m_wheelConnectionWidth *= CAR_SCALE;
			m_wheelConnectionLength *= CAR_SCALE;
			m_bodyToChasisHeight *= CAR_SCALE; 
		}
	}; 
}

enum Wheels{FRONTLEFT, FRONTRIGHT, REARLEFT, REARRIGHT};

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
	void UpdateMatrix(glm::mat4 matrix, glm::vec3 scale);
	glm::mat4 GetCarMatrix();
	glm::mat4 GetWorldPos();
	//glm::mat4 getWheelWorldPos();

	void Drive();
	void Reverse();
	void TurnLeft();
	void TurnRight();
	void Brake();
	
	btQuaternion GetRotationQuatFromAngle(const btVector3& axis, btScalar angle);
	
	//btRigidBody* LocalCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colShape);
	// 

private:
	std::shared_ptr<GRAPHICS::Model> m_car = nullptr;
	std::shared_ptr<GRAPHICS::Model> m_carWheels = nullptr;
	std::vector<std::shared_ptr<GRAPHICS::Model>> m_carWheelsPtr;
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
	bool m_turning;

	glm::mat4 m_carModelMatrix;

	Wheels m_carWheelsEnum;

	int m_wheelInstances[4];
};