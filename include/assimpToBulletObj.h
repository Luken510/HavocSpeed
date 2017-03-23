#pragma once

#include <LinearMath/btAlignedObjectArray.h>
#include <btBulletDynamicsCommon.h>
#include <glm\glm.hpp>

#include "modelLoader.h"
#include "meshLoader.h"

//example RigidBodyFromObj - reference
namespace GRAPHICS {

	struct ObjInstanceVertex
	{
		float xyz[3];
		float normal[3]; //add if needed
		float uv[2];
	};
	struct ObjInstanceShape
	{
		btAlignedObjectArray<ObjInstanceVertex>* m_vertices = nullptr; //obj array used to replace vector
		int m_numOfVertices;
		btAlignedObjectArray<int>* m_indices = nullptr;
		int m_numOfIndices;
		float m_scaling[4];

		ObjInstanceShape() :m_vertices(0), m_indices(0)
		{

		
		}

		/*virtual ~ObjInstanceShape()
		{
			delete m_vertices;
			delete m_indices; 
		}*/
	};


	std::shared_ptr<ObjInstanceShape> AssimpToBulletObj(const std::vector<GRAPHICS::Mesh>& meshes);

}