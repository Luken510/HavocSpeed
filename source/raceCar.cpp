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
	m_carRwheels = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearWheels.obj");//try to break these up into 2 wheels??????????
	m_carFrontR = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontRightWheel.obj");
	m_carFrontL = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontLeftWheel.obj"); // similar to the robot, built car class and init them all in there.

	
}

void RaceCar::CreateCarBulletObjFromModel()
{
	//body/chasis
	std::shared_ptr<GRAPHICS::ObjInstanceShape> glmesh = GRAPHICS::AssimpToBulletObj(m_car->GetMeshes());
	const GRAPHICS::ObjInstanceVertex& vertices = glmesh->m_vertices->at(0);
	carChasis = PHYSICS::PhysicsController::getPhysicsInstance().CreateConvexHull(vertices, glmesh->m_numOfVertices, sizeof(GRAPHICS::ObjInstanceVertex), 0.1f, 1.0f);
	btScalar mass(1.0f);
	//starting Pos
	btTransform startingPos;
	startingPos.setIdentity();
	btVector3 Pos(45.0f, 1.0f, 0.0f);
	startingPos.setOrigin(Pos);
	PHYSICS::PhysicsController::getPhysicsInstance().AddModel(carChasis, startingPos, mass);


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
	m_carRwheels->Render(shader);
	m_carFrontR->Render(shader);
	m_carFrontL->Render(shader);
	
}

btRigidBody * RaceCar::localCreateRigidBody(btScalar mass, const btTransform & worldTransform, btCollisionShape * colShape)
{
	return nullptr;
}
