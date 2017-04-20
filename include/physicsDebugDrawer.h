#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <btBulletDynamicsCommon.h>

namespace GRAPHICS
{
	/*! \struct Vertex
	\brief To store the positions of physic models.
	*/
	struct Line {
		Line(glm::vec3 startLine, glm::vec3 finishLine, glm::vec3 colourLine) : start(startLine), finish(finishLine), colour(colourLine) {};

		glm::vec3 start, finish, colour;		
	};
	
	class PhysicsDebugDrawer : public btIDebugDraw
	{
	public:
		/*! 
		\brief Constructor
		*/
		PhysicsDebugDrawer();
		/*!
		\brief Destructor
		*/
		~PhysicsDebugDrawer();

		/*!
		\brief Drawline function, needed for Physics Debug
		*/
		virtual void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color);
		/*!
		\brief setDebugMode function, needed for Physics Debug
		*/
		virtual void setDebugMode(int debugMode);
		/*!
		\brief getDebugMode function, needed for Physics Debug
		*/
		virtual int getDebugMode() const;
		/*!
		\brief drawContactPoint function, needed for Physics Debug
		*/
		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
		/*!
		\brief drawTriangle function, needed for Physics Debug
		*/
		virtual void drawTriangle(const btVector3 & a, const btVector3 & b, const btVector3 & c, const btVector3 & color, btScalar alpha);
		/*!
		\brief reportErrorWarning function, needed for Physics Debug
		*/
		void reportErrorWarning(const char * warningString);
		/*!
		\brief draw3dText function, needed for Physics Debug
		*/
		virtual void draw3dText(const btVector3& location, const char* textString);
	
		/*!
		\brief GetLines function, needed for rendering lines
		*/
		std::vector<GRAPHICS::Line>& GetLines();
		/*!
		\brief Clear's lines to avoid memory leak.
		*/
		void ClearLines();

	private:

		std::vector<Line> m_lines; //!< vector of line structs, to be used to draw
		int m_debugMode; //!< bool, if true draw wireframemode
	};
}