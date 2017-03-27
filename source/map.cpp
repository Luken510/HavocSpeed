#include "..\include\map.h"

#include <memory>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <math.h>
#include "logger.h"

GAME::Map::Map() : m_trackModelMatrix(1.0f)
{

	
}

void GAME::Map::Init()
{
	m_background = std::make_shared<GRAPHICS::Model>("./external/assets/map/racetrack/racetrackwithBG.3ds");
	m_track = std::make_shared<GRAPHICS::Model>("./external/assets/map/racetrack/racetrackwobg.3ds");

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
	UpdateMatrix(GetWorldPos(), glm::vec3(MAP_SCALE));
}

void GAME::Map::Render(std::shared_ptr<GRAPHICS::Shader> shader)
{
	m_background->Render(shader);
	m_track->Render(shader);
}


void GAME::Map::CreateMapObjects()
{
	//track imported into bullet for physics;
	std::shared_ptr<GRAPHICS::ObjInstanceShape> glmesh = GRAPHICS::AssimpToBulletObj(m_track->GetMeshes());
	btAlignedObjectArray<GRAPHICS::ObjInstanceVertex>* vertices = glmesh->m_vertices;
	btAlignedObjectArray<int>* indices = glmesh->m_indices;

	m_carTrackMesh = PHYSICS::PhysicsController::GetPhysicsInstance().CreateConvexHull(
		vertices,
		glmesh->m_numOfVertices,
		sizeof(GRAPHICS::ObjInstanceVertex),
		MAP_SCALE,
		0,
		true
		);

	PHYSICS::PhysicsController::GetPhysicsInstance().AddModel(m_carTrackMesh);
	btTransform trackTransform;
	trackTransform.setIdentity();
	trackTransform.setOrigin(btVector3(0.0f, -100.0f, 0.0f));
	btQuaternion trackRotate;
	trackRotate.setRotation(btVector3(1.0f, 0.0f, 0.0f), -89.55f);
	trackTransform.setRotation(trackRotate);

	m_staticTrack = PHYSICS::PhysicsController::GetPhysicsInstance().CreateRigidbody(0, trackTransform, m_carTrackMesh);
	PHYSICS::PhysicsController::GetPhysicsInstance().AddRigidBody(m_staticTrack);




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
	btTransform translateToWorld;
	m_staticTrack->getMotionState()->getWorldTransform(translateToWorld);
	btScalar* OpenGLMatrix = new btScalar[16];
	translateToWorld.getOpenGLMatrix(OpenGLMatrix);

	return glm::make_mat4(OpenGLMatrix);
}
