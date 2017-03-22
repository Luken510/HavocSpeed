#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <btBulletDynamicsCommon.h>

namespace GRAPHICS
{
	struct Line {
		Line(glm::vec3 startLine, glm::vec3 finishLine, glm::vec3 colourLine) : start(startLine), finish(finishLine), colour(colour) {};

		glm::vec3 start, finish, colour;		
	};
	
	class PhysicsDebugDrawer : public btIDebugDraw
	{
	public:

		PhysicsDebugDrawer();
		~PhysicsDebugDrawer();

		virtual void DrawLine(const btVector3 & start, const btVector3 & finish, const btVector3 & colour);
		virtual void SetDebug(int debugMode);
		virtual int getDebugMode();

		std::vector<GRAPHICS::Line> GetLines();

	private:

		std::vector<Line> m_lines;
		int m_debugMode;
	};
}