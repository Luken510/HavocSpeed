#include "physicsDebugDrawer.h"
#include <btBulletDynamicsCommon.h>
#include "logger.h"

GRAPHICS::PhysicsDebugDrawer::PhysicsDebugDrawer() : m_debugMode(1)
{

}

GRAPHICS::PhysicsDebugDrawer::~PhysicsDebugDrawer()
{

}

void GRAPHICS::PhysicsDebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	GRAPHICS::Line m(glm::vec3(from.x(), from.y(), from.z()),
					 glm::vec3(to.x(), to.y(), to.z()),
					 glm::vec3(color.x(), color.y(), color.z()));
	m_lines.push_back(m);
}

void GRAPHICS::PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int GRAPHICS::PhysicsDebugDrawer::getDebugMode() const
{
	return m_debugMode;
}

void GRAPHICS::PhysicsDebugDrawer::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
}

void GRAPHICS::PhysicsDebugDrawer::drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha)
{
}

void GRAPHICS::PhysicsDebugDrawer::reportErrorWarning(const char * warningString)
{
	UTIL::LOG(UTIL::LOG::WARNING) << "Wire Frame Debugger WARNING: " << warningString;
}

void GRAPHICS::PhysicsDebugDrawer::draw3dText(const btVector3 & location, const char * textString)
{
}
std::vector<GRAPHICS::Line>& GRAPHICS::PhysicsDebugDrawer::GetLines()
{
	return m_lines;
}

void GRAPHICS::PhysicsDebugDrawer::ClearLines()
{
	m_lines.clear();
}
