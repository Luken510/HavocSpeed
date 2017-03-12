#include "..\include\raceCar.h"

RaceCar::RaceCar()
{




	m_car = std::make_shared<GRAPHICS::Model>("./external/assets/car/CarChasis.obj");
	m_carRwheels = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearWheels.obj");
	m_carFrontR = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontRightWheel.obj");
	m_carFrontL = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontLeftWheel.obj"); // similar to the robot, built car class and init them all in there.
	
}

RaceCar::~RaceCar()
{
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
