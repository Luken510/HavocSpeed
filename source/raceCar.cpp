#include "raceCar.h"
#include "assimpToBulletObj.h"
#include "logger.h"



RaceCar::RaceCar()
{
	m_car = std::make_shared<GRAPHICS::Model>("./external/assets/car/CarChasis.obj");
	m_carRwheels = std::make_shared<GRAPHICS::Model>("./external/assets/car/rearWheels.obj");
	m_carFrontR = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontRightWheel.obj");
	m_carFrontL = std::make_shared<GRAPHICS::Model>("./external/assets/car/frontLeftWheel.obj"); // similar to the robot, built car class and init them all in there.
	
	
	m_carTriMeshShape;
	m_carConvexHullShape;
	m_carTestTri;

	m_carCompound;

	m_carRayCaster;
	m_rayCar;
	m_carChassis;

	std::shared_ptr<GRAPHICS::ObjInstanceShape> glmesh = GRAPHICS::AssimpToBulletObj(m_car->GetMeshes());
	UTIL::LOG(UTIL::LOG::INFO) << "Obj Loaded: number of extracted vertices :" << glmesh->m_numOfVertices;
	UTIL::LOG(UTIL::LOG::INFO) << "Obj Loaded: number of extracted vertices :" << glmesh->m_numOfIndices; //check to see if its doing whats its meant to , as indicies = verticies, check to see if indices actually do = vertices

	const GRAPHICS::ObjInstanceVertex& v = glmesh->m_vertices->at(0);
	btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&(v.xyzw[0])), glmesh->m_numOfVertices, sizeof(GRAPHICS::ObjInstanceVertex));
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
