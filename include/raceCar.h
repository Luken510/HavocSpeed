#pragma once


#include "entity.h"
#include "modelLoader.h"
#include "meshLoader.h"
#include "PhysicsController.h"
#include "eventHandler.h"
#include "rocket.h"

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES

#define CAR_SCALE 0.15f


namespace CarConfig
{
	struct Config
	{
		btScalar	m_mass; //!< mass of the car
		btScalar	m_dampingLinear;  //!< Linear Damping of the chasis
		btScalar	m_dampingAngle;  //!< Angular Damping of the chasis
		btScalar	m_maxEnginePower;  //!< Max amount of torque applied to the wheels
		btScalar	m_maxBreakingPower; //!< Max amount of breaking torque applied to the wheels
		btScalar	m_maxSpeed; //!< max speed the car can achieve (km/h) - km cause of bullet
		btScalar	m_turningIncrement; //!< how fast the car turns
		btScalar	m_turningDecrement; //!< how fast the car resets
		btScalar	m_turningLimits; //!< max angle the car can turn
		btScalar	m_wheelFriction;  //!< friction between the tyre and ground
		btScalar	m_wheelRadius;  //!< wheel radius
		btScalar	m_wheelWidth; //!< wheel width
		btScalar	m_wheelConnectionHeight; //!< starting point of the ray in chasis space (y)
		btScalar	m_wheelConnectionWidth; //!< starting point of the ray in chasis space (x)
		btScalar	m_wheelConnectionLength; //!< starting point of the ray in chasis space (z)
		btScalar	m_suspensionStiffness; //!< stiffness constant for suspension. 10 - off road, 50 - sports car, 200 - f1 car
		btScalar	m_suspensionDampRelaxtion; //!< damping coefficient for when the suspension is expanding
		btScalar	m_suspensionDampCompression;//!< damping coefficient for when the suspension is compressed
		btScalar	m_suspensionMaxLength; //!< the max length of the suspension (metres)
		btScalar	m_suspensionMaxTravel; //!< the maximum distance the suspension can be compressed
		btScalar	m_rollInfluence; //!< reduces the rolling torque applied from the wheel that cause the vehicle to roll over - a slight hack
		btScalar	m_bodyToChasisHeight;

		btVector3 m_wheelDirection; //!< the direction of the ray cast in chasis space
		btVector3  wheelAxel; //!< the axis that the wheels rotate around

		Config() :
			m_mass(5.0f),
			m_dampingLinear(0.25f),
			m_dampingAngle(0.0f),
			m_maxEnginePower(-30.0f), //direction of movement
			m_maxBreakingPower(0.5f),
			m_maxSpeed(120.0f),
			m_turningIncrement(0.025f),
			m_turningDecrement(0.1f),
			m_turningLimits(0.50f),
			m_wheelFriction(2.5f),
			m_wheelRadius(3.302f),
			m_wheelWidth(2.215f),
			m_wheelConnectionHeight(3.9f), //x
			m_wheelConnectionWidth(6.200f), //y
			m_wheelConnectionLength(11.725f), //z
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

class RaceCar : public UTIL::Entity {

public:
	/*
	\brief Constructor
	*/
	RaceCar();
	RaceCar::RaceCar(bool player);
	/*
	\brief Destructor
	*/
	~RaceCar();

	/*!
	\brief Update Update the Entity, main game logic function
	\param deltaTime current time at the frame.
	*/
	virtual void Update(double deltaTime);

	/*!
	\brief Render rendering the object using a shader
	\param shader the shader to render to
	*/
	virtual void Render(std::shared_ptr<GRAPHICS::Shader> shader);


	/*!
	\brief Init initialise the Entity.
	*/
	virtual void Init();


	/*!
	\brief CreateCarBulletObjFromModel initialise the car model into bullet.
	*/
	void CreateCarBulletObjFromModel();

	/*!
	\brief UpdateMatrix function to update the model matrix of an enitity - used to help render models
	\param Pos position to set the model matrix to
	\param scale to scale the model matrix by
	*/
	virtual void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale);
	/*!
	\brief UpdateMatrix function to update the model matrix of an enitity - used to help render models
	\param Pos position to set the matrix matrix to
	\param scale to scale the model matrix  by
	\param rotateAxis which axis to rotate current model matrix by
	\param angle angle to rotate the model matrix by
	*/
	virtual void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle);

	/*!
	\brief UpdateMatrix function to update the matrix of an enitity - used to help render models
	\param matrix position and rotation to set the model matrix to
	\param scale to scale the model matrix by
	*/
	virtual void UpdateMatrix(glm::mat4 matrix, glm::vec3 scale);

	/*!
	\brief GetMatrix virtual function to retreive an entities matrix.
	*/
	virtual glm::mat4 GetMatrix();

	/*!
	\brief GetWorldPos virtual function to retreive an entities world position.
	*/
	virtual glm::mat4 GetWorldPos();

	/*!
	\brief GetVelocity virtual function to retreive an entities Velocity.
	*/
	virtual	glm::vec3 GetVelocity();

	/*!
	\brief Drive drive the car
	*/
	void Drive();
	/*!
	\brief Reverse Reverse the car 
	*/
	void Reverse();
	/*!
	\brief TurnLeft turn the car left
	*/
	void TurnLeft();
	/*!
	\brief TurnRight turn the car right
	*/
	void TurnRight();
	/*!
	\brief Brake apply brakes to the car
	*/
	void Brake();
	/*!
	\brief Fire fire the weapon
	*/
	void Fire();
	
	/*!
	\brief GetRotationQuatFromAngle function to retrieve a rotate in quaternions from an axis and angle
	\param axis the axis to rotate by
	\param angle the angle to rotate by
	*/
	virtual btQuaternion GetRotationQuatFromAngle(const btVector3& axis, btScalar angle);


private:
	std::shared_ptr<GRAPHICS::Model> m_car = nullptr; //!<Model for the car
	std::shared_ptr<GRAPHICS::Model> m_carWheels = nullptr; //!< model for the cars wheels
	std::vector<std::shared_ptr<GRAPHICS::Model>> m_carWheelsPtr; //!< vector to pointer to the loaded car wheels model

	std::shared_ptr<GRAPHICS::RocketModel> m_rocketsModel = nullptr; //!< Rocket model set up for flyweight
	//just for testing have a vector of rockets
	std::vector<std::shared_ptr<WEAPONS::Rocket>> m_rockets; //!< vector of rockets to be rendered without flyweight
	std::vector<std::shared_ptr<WEAPONS::Rocket>> m_rocketsFly; //!< vector of rockets to be rendered with flyweight
	

	CarConfig::Config carConfig; //!< Struct containing the car configuation (speed etc.)

	btConvexHullShape* m_carChasis = nullptr; //!< btConvexHull Shape for the car chasis
	btCompoundShape* m_carChasisComp = nullptr; //!< m_carChasisComp Shape for the car chasis
	btVehicleRaycaster* m_carRayCaster = nullptr; //!< btVehicleRaycaster Shape for the car chasis
	btRaycastVehicle* m_raycastCar = nullptr; //!< btRaycastVehicle Main car object to be manipulated - a raycastvehicle in bullet
	btRigidBody* m_carChassisRigid = nullptr; //!< btRigidBody Shape for the car chasis

	float m_enginePower; //!< engine power to drive the car
	float m_brakingPower; //!< breaking power to stop the car
	float m_steeringPower; //!< steering power to turn the car
	bool m_turning; //!< whether the car is turning or not
	bool m_rocketFired; //!< whether the rocket has be fired or not - used for delay
	bool player2; //!< if this car being generated is player2

	glm::mat4 m_carModelMatrix; //!< model matrix for the car

	Wheels m_carWheelsEnum; //!< enum to set the wheels correctly

	int m_wheelInstances[4]; //!< int to set the wheels correctly
};