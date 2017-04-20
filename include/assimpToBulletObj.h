#pragma once

#include <LinearMath/btAlignedObjectArray.h>
#include <btBulletDynamicsCommon.h>
#include <glm\glm.hpp>

#include "modelLoader.h"
#include "meshLoader.h"

namespace GRAPHICS {

	/*! \struct ObjInstanceVertex
	\brief To store the models Position, position normal, and Texture co-ords.
	*/
	struct ObjInstanceVertex
	{
		float xyz[3];//!< vertices of the meshs
		float normal[3]; //!<  normals of the meshs
		float uv[2];//!<  Texture coordinates of the meshs
	};

	/*!
	\brief Convert Assimp's model into a bullet object.
	*/
	struct ObjInstanceShape
	{
		btAlignedObjectArray<ObjInstanceVertex>* m_vertices = nullptr; //!< obj array used to replace vector
		int m_numOfVertices;//!<  number of vertices
		btAlignedObjectArray<unsigned int>* m_indices = nullptr;//!<  Texture coordinates of the meshs
		int m_numOfIndices;//!<  number of indices
		float m_scaling[4];//!<  scaling of the mesh

		/*!
		\brief Constructor
		*/
		ObjInstanceShape() :m_vertices(0), m_indices(0)
		{

		
		}
		/*!
		\brief Destructor
		*/
		virtual ~ObjInstanceShape()
		{
			delete m_vertices;
			delete m_indices; 
		}
	};

	/*!
	\brief Convert Assimp's model into a bullet object.
	\param meshes Assimp's mesh to be converted 
	*/
	std::shared_ptr<ObjInstanceShape> AssimpToBulletObj(const std::vector<GRAPHICS::Mesh>& meshes);

}