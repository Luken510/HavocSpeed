#include "raceCar.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <math.h>
#include "assimpToBulletObj.h"
#include "logger.h"
#include "PhysicsController.h"



RaceCar::RaceCar() : m_enginePower(0),m_turning(0), m_brakingPower(0), m_steeringPower(0), m_carModelMatrix(1.0f)
{

	m_carModelMatrix = glm::scale(glm::vec3(CAR_SCALE));
}

RaceCar::~RaceCar()
{
}

void RaceCar::Init()
{
	
	m_car = std::make_shared<GRAPHICS::Model>("./external/assets/car/CarChasis.obj");
	m_carWheels = std::make_shared<GRAPHICS::Model>("./external/assets/car/carWheels.obj");

	CreateCarBulletObjFromModel();

	m_carChasisComp = new btCompoundShape();

	btTransform startingPos;
	startingPos.setOrigin(btVector3(40.0f, 100.4f, -55.0f));
	startingPos.setRotation(GetRotationQuatFromAngle(btVector3(1.0f, 0.0f, 0.0f), 90.0)); // changes the direction it faces  WHY DOES THIS BREAK IT, PLS CHECK TY.

	btTransform localTrans;//
	localTrans.setIdentity();//
	btVector3 Pos(0.0f, carConfig.m_bodyToChasisHeight, 0.0f); // change to set positions in car info struct or as a passed param in init
	UpdateMatrix(glm::vec3(Pos.x(),Pos.y(), Pos.z()), glm::vec3(CAR_SCALE));
	localTrans.setOrigin(Pos);
	m_carChasisComp->addChildShape(localTrans, m_carChasis);


	m_carChassisRigid = PHYSICS::PhysicsController::GetPhysicsInstance().CreateRigidbody(carConfig.m_mass, startingPos, m_carChasisComp);
	m_carChassisRigid->setDamping(carConfig.m_dampingLinear, carConfig.m_dampingAngle);
	PHYSICS::PhysicsController::GetPhysicsInstance().AddRigidBody(m_carChassisRigid);


	//create the bullet vehicle
	m_carRayCaster = new btDefaultVehicleRaycaster(PHYSICS::PhysicsController::GetPhysicsInstance().GetDynamicWorld());

	btRaycastVehicle::btVehicleTuning m_tuning;
	m_raycastCar = new btRaycastVehicle(m_tuning, m_carChassisRigid, m_carRayCaster);
	PHYSICS::PhysicsController::GetPhysicsInstance().GetDynamicWorld()->addVehicle(m_raycastCar);

	m_carChassisRigid->setActivationState(DISABLE_DEACTIVATION);
	m_raycastCar->setCoordinateSystem(0, 1, 2);

	//add wheels
	btVector3 WheelConnectionsParams;
	
	
	//rear left wheel
	WheelConnectionsParams = btVector3(carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, carConfig.m_wheelConnectionLength);
	m_raycastCar->addWheel(WheelConnectionsParams, carConfig.m_wheelDirection, carConfig.wheelAxel, carConfig.m_suspensionMaxLength, carConfig.m_wheelRadius, m_tuning, true);

	//rear right wheel
	WheelConnectionsParams = btVector3(-carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, carConfig.m_wheelConnectionLength);
	m_raycastCar->addWheel(WheelConnectionsParams, carConfig.m_wheelDirection, carConfig.wheelAxel, carConfig.m_suspensionMaxLength, carConfig.m_wheelRadius, m_tuning, true);

	//front left wheel
	WheelConnectionsParams = btVector3(carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, -carConfig.m_wheelConnectionLength + 0.3f);
	m_raycastCar->addWheel(WheelConnectionsParams, carConfig.m_wheelDirection, carConfig.wheelAxel, carConfig.m_suspensionMaxLength, carConfig.m_wheelRadius, m_tuning, false);

	//front right wheel
	WheelConnectionsParams = btVector3(-carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, -carConfig.m_wheelConnectionLength + 0.3f);
	m_raycastCar->addWheel(WheelConnectionsParams, carConfig.m_wheelDirection, carConfig.wheelAxel, carConfig.m_suspensionMaxLength, carConfig.m_wheelRadius, m_tuning, false);

	for (int i = 0; i < m_raycastCar->getNumWheels(); i++)
	{
		btWheelInfo wheelInfo = m_raycastCar->getWheelInfo(i);

		wheelInfo.m_suspensionStiffness = carConfig.m_suspensionStiffness;
		wheelInfo.m_wheelsDampingRelaxation = carConfig.m_suspensionDampRelaxtion;
		wheelInfo.m_wheelsDampingCompression = carConfig.m_suspensionDampCompression;
		wheelInfo.m_frictionSlip = carConfig.m_wheelFriction;
		wheelInfo.m_rollInfluence = carConfig.m_rollInfluence;
		wheelInfo.m_maxSuspensionTravelCm = carConfig.m_suspensionMaxTravel;
		m_carWheelsPtr.push_back(m_carWheels);
	}
}

void RaceCar::CreateCarBulletObjFromModel()
{
	//body/chasis
	std::shared_ptr<GRAPHICS::ObjInstanceShape> glmesh = GRAPHICS::AssimpToBulletObj(m_car->GetMeshes());
	btAlignedObjectArray<GRAPHICS::ObjInstanceVertex>* vertices = glmesh->m_vertices;

	m_carChasis = PHYSICS::PhysicsController::GetPhysicsInstance().CreateConvexHull(vertices,
																					glmesh->m_numOfVertices, 
																					sizeof(GRAPHICS::ObjInstanceVertex), 
																					CAR_SCALE, 
																					carConfig.m_mass,
																					false);
}

void RaceCar::Update(double deltaTime)
{
	
	
	m_raycastCar->applyEngineForce(m_enginePower, REARLEFT); 
	m_raycastCar->applyEngineForce(m_enginePower, REARRIGHT); 

	m_raycastCar->setBrake(m_brakingPower, REARLEFT);
	m_raycastCar->setBrake(m_brakingPower, REARRIGHT);

	m_raycastCar->setSteeringValue(m_steeringPower, REARLEFT);
	m_raycastCar->setSteeringValue(m_steeringPower, REARRIGHT);

	m_enginePower = 0.0f; // reset the power to be set again with the key input
	m_brakingPower = 0.0f;

	if (!m_turning)
	{
		if (m_steeringPower > 0.f)
		{
			m_steeringPower -= carConfig.m_turningDecrement;
			if (m_steeringPower < 0.f)
				m_steeringPower = 0.0f;
		}
		else
		{
			m_steeringPower += carConfig.m_turningIncrement;
			if( m_steeringPower > 0.f)
				m_steeringPower = 0.f;
		}
	}

	m_turning = false;
	
	//update car position for its model
	UpdateMatrix(GetWorldPos(), glm::vec3(CAR_SCALE));
}

void RaceCar::Render(std::shared_ptr<GRAPHICS::Shader> shader)
{
	m_car->Render(shader);
	

	for (int i = 0; i < m_raycastCar->getNumWheels(); i++)
	{
		m_raycastCar->updateWheelTransform(i, true);
		glm::mat4 setWheelModel = glm::mat4(1.0f) * glm::scale(
			PHYSICS::PhysicsController::GetPhysicsInstance().btTransToGlmMat4(m_raycastCar->getWheelInfo(i).m_worldTransform),
			glm::vec3(CAR_SCALE)); // 

		if (i == FRONTLEFT || i == REARLEFT)
			setWheelModel = glm::rotate(setWheelModel, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		shader->SetUniform("model", setWheelModel);
		m_carWheelsPtr[i]->Render(shader);
	}
	
}

void RaceCar::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale)
{
	m_carModelMatrix = glm::translate(Pos) * glm::scale(scale);
}

void RaceCar::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle)
{
	m_carModelMatrix = glm::translate(Pos) * glm::rotate(glm::radians(angle), rotateAxis) * glm::scale(scale);
}

void RaceCar::UpdateMatrix(glm::mat4 matrix, glm::vec3 scale)
{
	m_carModelMatrix = matrix * glm::scale(scale); // loses scale so re-adding
}

glm::mat4 RaceCar::GetCarMatrix()
{
	return m_carModelMatrix;
}

glm::mat4 RaceCar::GetWorldPos()
{
	btTransform translateToWorld;
	m_raycastCar->getRigidBody()->getMotionState()->getWorldTransform(translateToWorld);
	btCompoundShape* compoundShape = static_cast<btCompoundShape*>(m_raycastCar->getRigidBody()->getCollisionShape());
	translateToWorld = translateToWorld * compoundShape->getChildTransform(0);
	btScalar Test1[16];
	btScalar* OpenGLMatrix = Test1;
	translateToWorld.getOpenGLMatrix(OpenGLMatrix);
	// had a memory leak, show in testing
	return glm::make_mat4(Test1);
}

glm::vec3 RaceCar::GetVelocity()
{
	btVector3 velocity = m_raycastCar->getRigidBody()->getLinearVelocity();
	
	return glm::vec3(velocity.x(), velocity.y(), velocity.z());
}

void RaceCar::Drive()
{
	if (std::abs(m_raycastCar->getCurrentSpeedKmHour()) < carConfig.m_maxSpeed)
		m_enginePower = carConfig.m_maxEnginePower;


	m_brakingPower = 0.0f;
}

void RaceCar::Reverse()
{

	if (std::abs(m_raycastCar->getCurrentSpeedKmHour()) < carConfig.m_maxSpeed)
		m_enginePower = -carConfig.m_maxEnginePower;

	m_brakingPower = 0.0f;
}

void RaceCar::TurnLeft()
{

	if (m_steeringPower < 0)
		m_steeringPower += carConfig.m_turningDecrement;
	else
		m_steeringPower += carConfig.m_turningIncrement;

	if (m_steeringPower > carConfig.m_turningLimits)
		m_steeringPower = carConfig.m_turningLimits;

	m_turning = true;
	
}

void RaceCar::TurnRight()
{
	
	if (m_steeringPower > 0)
		m_steeringPower -= carConfig.m_turningDecrement;
	else
		m_steeringPower -= carConfig.m_turningIncrement;

	if (m_steeringPower < -carConfig.m_turningLimits)
		m_steeringPower = -carConfig.m_turningLimits;

	m_turning = true;
	
}

void RaceCar::Brake()
{
	
		m_brakingPower = carConfig.m_maxBreakingPower;
		m_enginePower = 0.0f;
}

btQuaternion RaceCar::GetRotationQuatFromAngle(const btVector3& axis, btScalar angle)
{

	btQuaternion rotation;
	
	rotation.setRotation(axis, btRadians(angle));

	return rotation;
	
}

/*btRigidBody * RaceCar::LocalCreateRigidBody(btScalar mass, const btTransform & worldTransform, btCollisionShape * colShape)
{
	return nullptr;
}*/
