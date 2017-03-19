#include "raceCar.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "assimpToBulletObj.h"
#include "logger.h"
#include "PhysicsController.h"



RaceCar::RaceCar()
{

	m_carModelMatrix = glm::translate(glm::vec3(45.0f, 1.0f, 0.0f)) * glm::scale(m_carScale); // default pos to draw 

}

RaceCar::~RaceCar()
{
}

void RaceCar::Init()
{
	
	m_car = std::make_shared<GRAPHICS::Model>("./external/assets/car/CarChasis.obj");
	m_carRearR = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearRightWheel.obj");;
	m_carRearL = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearLeftWheel.obj");;
	m_carFrontR = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontRightWheel.obj");
	m_carFrontL = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontLeftWheel.obj"); 

	CreateCarBulletObjFromModel();

	m_carChasisComp = new btCompoundShape();

	btScalar mass(1.0f);//
	//starting Pos//
	btTransform startingPos;//
	startingPos.setIdentity();//
	btVector3 Pos(45.0f, 1.0f, 0.0f); // change to set positions in car info struct or as a passed param in init
	UpdateMatrix(glm::vec3(Pos.x(),Pos.y(), Pos.z()), m_carScale);
	startingPos.setOrigin(Pos);
	m_carChasisComp->addChildShape(startingPos, m_carChasis);
	//PHYSICS::PhysicsController::getPhysicsInstance().AddModel(m_carChasisComp);
	m_carChassisRigid = PHYSICS::PhysicsController::GetPhysicsInstance().CreateRigidbody(mass, startingPos, m_carChasisComp);
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
	
	
	//front left wheel
	WheelConnectionsParams = btVector3(carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, carConfig.m_wheelConnectionLength);
	m_raycastCar->addWheel(WheelConnectionsParams, carConfig.m_wheelDirection, carConfig.wheelAxel, carConfig.m_suspensionMaxLength, carConfig.m_wheelRadius, m_tuning, true);

	//front right wheel
	WheelConnectionsParams = btVector3(-carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, carConfig.m_wheelConnectionLength);
	m_raycastCar->addWheel(WheelConnectionsParams, carConfig.m_wheelDirection, carConfig.wheelAxel, carConfig.m_suspensionMaxLength, carConfig.m_wheelRadius, m_tuning, true);

	//rear left wheel
	WheelConnectionsParams = btVector3(carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, -carConfig.m_wheelConnectionLength);
	m_raycastCar->addWheel(WheelConnectionsParams, carConfig.m_wheelDirection, carConfig.wheelAxel, carConfig.m_suspensionMaxLength, carConfig.m_wheelRadius, m_tuning, false);

	//rear right wheel
	WheelConnectionsParams = btVector3(-carConfig.m_wheelConnectionWidth, carConfig.m_wheelConnectionHeight, -carConfig.m_wheelConnectionLength);
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
	}
}

void RaceCar::CreateCarBulletObjFromModel()
{
	//body/chasis
	std::shared_ptr<GRAPHICS::ObjInstanceShape> glmesh = GRAPHICS::AssimpToBulletObj(m_car->GetMeshes());
	const GRAPHICS::ObjInstanceVertex& vertices = glmesh->m_vertices->at(0);
	m_carChasis = PHYSICS::PhysicsController::GetPhysicsInstance().CreateConvexHull(vertices, glmesh->m_numOfVertices, sizeof(GRAPHICS::ObjInstanceVertex), 0.1f, 1.0f);
	//rear Wheels


	//Front Right Wheel
	//Front Left Wheel

}

void RaceCar::Update(float deltaTime)
{
	m_enginePower = carConfig.m_maxEnginePower;
	
	m_raycastCar->applyEngineForce(m_enginePower, 2); // WHEEL rearLEFt, - SET UP AN ENUM ( 0, 1, 2,3)
	m_raycastCar->applyEngineForce(m_enginePower, 3); // WHEEL rearRIGHT

	m_raycastCar->setBrake(m_brakingPower, 2);
	m_raycastCar->setBrake(m_brakingPower, 3);

	m_raycastCar->setSteeringValue(m_steeringPower, 2);
	m_raycastCar->setSteeringValue(m_steeringPower, 3);

	m_enginePower = 0.0f; // reset the power to be set again with the key input
	m_brakingPower = 0.0f;

	//do turning decrease/increase?;
	
	//update model pos
	
	UpdateMatrix(getWorldPos());
}

void RaceCar::Render(std::shared_ptr<GRAPHICS::Shader> shader)
{
	m_car->Render(shader);
	m_carRearR->Render(shader);
	m_carRearL->Render(shader);
	m_carFrontR->Render(shader);
	m_carFrontL->Render(shader);
	
}

void RaceCar::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale)
{
	m_carModelMatrix = glm::translate(Pos) * glm::scale(scale);
}

void RaceCar::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle)
{
	m_carModelMatrix = glm::translate(Pos) * glm::rotate(glm::radians(angle), rotateAxis) * glm::scale(scale);
}

void RaceCar::UpdateMatrix(glm::mat4 matrix)
{
	m_carModelMatrix = matrix * glm::scale(m_carScale); // loses scale so re-adding
}

glm::mat4 RaceCar::GetCarMatrix()
{
	return m_carModelMatrix;
}

glm::mat4 RaceCar::getWorldPos()
{
	btTransform translateToWorld;
	m_raycastCar->getRigidBody()->getMotionState()->getWorldTransform(translateToWorld);
	btCompoundShape* compoundShape = static_cast<btCompoundShape*>(m_raycastCar->getRigidBody()->getCollisionShape());
	translateToWorld = translateToWorld * compoundShape->getChildTransform(0);
	btScalar* m = new btScalar[16];
	translateToWorld.getOpenGLMatrix(m);
	
	return glm::make_mat4(m);
}

/*btRigidBody * RaceCar::LocalCreateRigidBody(btScalar mass, const btTransform & worldTransform, btCollisionShape * colShape)
{
	return nullptr;
}*/
