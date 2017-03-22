#include "physicsDebugDrawer.h"
#include <btBulletDynamicsCommon.h>

GRAPHICS::PhysicsDebugDrawer::PhysicsDebugDrawer() : m_debugMode(1)
{

}

GRAPHICS::PhysicsDebugDrawer::~PhysicsDebugDrawer()
{
}

void GRAPHICS::PhysicsDebugDrawer::DrawLine(const btVector3 & start, const btVector3 & finish, const btVector3 & colour)
{
	GRAPHICS::Line m(glm::vec3(start.x(), start.y(), start.z()), 
					 glm::vec3(finish.x(), finish.y(), finish.z()), 
					 glm::vec3(colour.x(), colour.y(), colour.z()));
	m_lines.push_back(m);
}

void GRAPHICS::PhysicsDebugDrawer::SetDebug(int debugMode)
{
	m_debugMode = debugMode;
}

int GRAPHICS::PhysicsDebugDrawer::getDebugMode()
{
	return m_debugMode;
}

std::vector<GRAPHICS::Line> GRAPHICS::PhysicsDebugDrawer::GetLines()
{
	return m_lines;
}
