#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <btBulletDynamicsCommon.h>

namespace GRAPHICS
{
	struct Line {
		Line(glm::vec3 startLine, glm::vec3 finishLine, glm::vec3 colourLine) : start(startLine), finish(finishLine), colour(colourLine) {};

		glm::vec3 start, finish, colour;		
	};
	
	class PhysicsDebugDrawer : public btIDebugDraw
	{
	public:

		PhysicsDebugDrawer();
		~PhysicsDebugDrawer();

		//needed functions for btIdebugDraw
		virtual void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color);
		virtual void setDebugMode(int debugMode);
		virtual int getDebugMode() const;
		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
		virtual void drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha);
		void reportErrorWarning(const char * warningString);
		virtual void draw3dText(const btVector3& location, const char* textString);
	
		std::vector<GRAPHICS::Line>& GetLines();
		void ClearLines();

	private:

		std::vector<Line> m_lines;
		int m_debugMode;
	};
}