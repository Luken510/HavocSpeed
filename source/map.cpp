#include "..\include\map.h"

#include <memory>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <math.h>
#include "logger.h"

GAME::Map::Map() 
{

<<<<<<< HEAD
	m_trackModelMatrix = glm::mat4(1.0f) * glm::translate(glm::vec3(0.0f, -100.f, 0.0f)) * glm::scale(glm::vec3(MAP_SCALE));
=======
	m_trackModelMatrix = glm::mat4(1.0f) * glm::translate(glm::vec3(0.0f, -100.f, 0.0f)) * glm::rotate(-89.55f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(MAP_SCALE));
>>>>>>> origin/master
}

void GAME::Map::Init()
{
<<<<<<< HEAD
	m_background = std::make_shared<GRAPHICS::Model>("./external/assets/map/obj/racetrack/racetrackwithBG.3ds");
	m_track = std::make_shared<GRAPHICS::Model>("./external/assets/map/obj/myracetracknoarrows.obj");
=======
	//m_background = std::make_shared<GRAPHICS::Model>("./external/assets/map/racetrack/racetrackwithBG.3ds");
	m_track = std::make_shared<GRAPHICS::Model>("./external/assets/map/test/5/4/objnoarrows/myracetrack.obj"); //obj might work as intended
>>>>>>> origin/master

	CreateMapObjects();

	
}

GAME::Map GAME::Map::GetCurrentMap()
{
	return Map();
}

void GAME::Map::SetMap()
{
}

void GAME::Map::Update(double deltaTime)
{
	//UpdateMatrix(GetWorldPos(), glm::vec3(MAP_SCALE));
}

void GAME::Map::Render(std::shared_ptr<GRAPHICS::Shader> shader)
<<<<<<< HEAD
{	
	m_background->Render(shader);
	//set Model again for background
=======
{//	m_background->Render(shader);
>>>>>>> origin/master
	m_track->Render(shader);
}


void GAME::Map::CreateMapObjects() 
{


	//m_carTrackModel = PHYSICS::PhysicsController::GetPhysicsInstance().CreateMultiCollisionShapes(m_track, btVector3(MAP_SCALE, MAP_SCALE, MAP_SCALE));


	//btTransform trackTransform;
	//trackTransform.setIdentity();
	//trackTransform.setOrigin(btVector3(0.0f, -100.0f, 0.0f));
	//btQuaternion trackRotate;
	//trackRotate.setRotation(btVector3(1.0f, 0.0f, 0.0f), -89.55f);
	//trackTransform.setRotation(trackRotate);

	//m_staticTrack = PHYSICS::PhysicsController::GetPhysicsInstance().CreateRigidbody(0, trackTransform, m_carTrackMesh);
	/*for (int i = 0; i < m_carTrackModel.size(); i++)
	{
		PHYSICS::PhysicsController::GetPhysicsInstance().AddStaticRigidBody(m_carTrackModel[i], trackTransform, btVector3(MAP_SCALE, MAP_SCALE, MAP_SCALE));
	}*/
	//track imported into bullet for physics;
	std::shared_ptr<GRAPHICS::ObjInstanceShape> glmesh = GRAPHICS::AssimpToBulletObj(m_track->GetMeshes());
	btAlignedObjectArray<GRAPHICS::ObjInstanceVertex>* vertices = glmesh->m_vertices;
	btAlignedObjectArray<unsigned int>* indices = glmesh->m_indices;

	m_carTrackMesh = PHYSICS::PhysicsController::GetPhysicsInstance().CreateConvexHull(m_track, MAP_SCALE, 0, false);

	btTransform trackTransform;
	trackTransform.setIdentity();
	trackTransform.setOrigin(btVector3(0.0f, -30.0f, 0.0f));
	btQuaternion trackRotate;
	trackRotate.setRotation(btVector3(1.0f, 0.0f, 0.0f), 0.0f);
	trackTransform.setRotation(trackRotate);

	for (int i = 0; i < m_carTrackMesh.size(); i++)
	{
		PHYSICS::PhysicsController::GetPhysicsInstance().AddModel(m_carTrackMesh[i]);
		m_staticTrack = PHYSICS::PhysicsController::GetPhysicsInstance().CreateRigidbody(0, trackTransform, m_carTrackMesh[i]);
		PHYSICS::PhysicsController::GetPhysicsInstance().AddRigidBody(m_staticTrack);
	}

	
	
}

void GAME::Map::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale)
{
	m_trackModelMatrix = glm::translate(Pos) * glm::scale(scale);
}

void GAME::Map::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle)
{
	m_trackModelMatrix = glm::translate(Pos) * glm::rotate(glm::radians(angle), rotateAxis) * glm::scale(scale);
}

void GAME::Map::UpdateMatrix(glm::mat4 matrix, glm::vec3 scale)
{
	m_trackModelMatrix = matrix * glm::scale(scale); // loses scale so re-adding
}

glm::mat4 GAME::Map::GetTrackMatrix()
{
	return m_trackModelMatrix;
}

glm::mat4 GAME::Map::GetWorldPos()
{
	/*btTransform translateToWorld;
	translateToWorld = m_staticTrack->getWorldTransform();
	btScalar Temp[16];
	btScalar* OpenGLMatrix = Temp;
	translateToWorld.getOpenGLMatrix(OpenGLMatrix);

	// had a memory leak, show in testing
	return glm::make_mat4(Temp);*/
	return glm::mat4(1.0f);
}
