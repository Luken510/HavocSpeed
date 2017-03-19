#include "raceCar.h"
#include "assimpToBulletObj.h"
#include "logger.h"
#include "PhysicsController.h"



RaceCar::RaceCar()
{



}

RaceCar::~RaceCar()
{
}

void RaceCar::init()
{
	
	m_car = std::make_shared<GRAPHICS::Model>("./external/assets/car/CarChasis.obj");
	m_carRearR = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearRightWheel.obj");;
	m_carRearL = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearLeftWheel.obj");;
	m_carFrontR = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontRightWheel.obj");
	m_carFrontL = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontLeftWheel.obj"); // similar to the robot, built car class and init them all in there.

	CreateCarBulletObjFromModel();

	m_carChasisComp = new btCompoundShape();

	btScalar mass(1.0f);//
	//starting Pos//
	btTransform startingPos;//
	startingPos.setIdentity();//
	btVector3 Pos(45.0f, 1.0f, 0.0f); // change to set positions in car info struct or as a passed param in init

	startingPos.setOrigin(Pos);
	m_carChasisComp->addChildShape(startingPos, m_carChasis);
	//PHYSICS::PhysicsController::getPhysicsInstance().AddModel(m_carChasisComp);
	m_carChassisRigid = PHYSICS::PhysicsController::GetPhysicsInstance().CreateRigidbody(mass, startingPos, m_carChasisComp);
	m_carChassisRigid->setDamping(carConfig.m_dampingLinear, carConfig.m_dampingAngle);
	PHYSICS::PhysicsController::GetPhysicsInstance().AddRigidBody(m_carChassisRigid);


	//create the bullet vehicle
	m_carRayCaster = new btDefaultVehicleRaycaster(PHYSICS::PhysicsController::GetPhysicsInstance().GetDynamicWorld());

	btRaycastVehicle::btVehicleTuning m_tuning;
	m_rayCar = new btRaycastVehicle(m_tuning, m_carChassisRigid, m_carRayCaster);
	PHYSICS::PhysicsController::GetPhysicsInstance().GetDynamicWorld()->addVehicle(m_rayCar);

	m_carChassisRigid->setActivationState(DISABLE_DEACTIVATION);
	m_rayCar->setCoordinateSystem(0, 1, 2);

	//add wheels
	//btVector3 WheelConnections();
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

void RaceCar::update(float deltaTime)
{

}

void RaceCar::render(std::shared_ptr<GRAPHICS::Shader> shader)
{
	m_car->Render(shader);
	m_carRearR->Render(shader);
	m_carRearL->Render(shader);
	m_carFrontR->Render(shader);
	m_carFrontL->Render(shader);
	
}

btRigidBody * RaceCar::localCreateRigidBody(btScalar mass, const btTransform & worldTransform, btCollisionShape * colShape)
{
	return nullptr;
}
