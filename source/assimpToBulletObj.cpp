#include "assimpToBulletObj.h"

#include <LinearMath/btAlignedObjectArray.h>
#include <btBulletDynamicsCommon.h>

std::shared_ptr<GRAPHICS::ObjInstanceShape> GRAPHICS::AssimpToBulletObj(const std::vector<GRAPHICS::Mesh>& meshes)
{
	//&getMeshes();
	
	//here we take the data from assimp's model, put it into our data structs to be fed into bullet.

	btAlignedObjectArray<ObjInstanceVertex>* verticesPtr(new btAlignedObjectArray<ObjInstanceVertex>);
	btAlignedObjectArray<int>* indicesPtr(new btAlignedObjectArray<int>);

		
		UTIL::LOG(UTIL::LOG::INFO) << "Obj Loaded: number of extracted Meshes :" << meshes.size();
			
		for (int i = 0; i < (int)meshes.size(); i++)
			{
				GRAPHICS::Mesh tempMesh = meshes[i];

				int vertCount = tempMesh.m_vertices.size();

				for (int v = 0; v < vertCount; v++)
				{
					ObjInstanceVertex vertex0;
					vertex0.xyzw[0] = tempMesh.m_vertices[v].m_position.x;
					vertex0.xyzw[1] = tempMesh.m_vertices[v].m_position.y;
					vertex0.xyzw[2] = tempMesh.m_vertices[v].m_position.z;
					vertex0.xyzw[3] = 0.f;

					vertex0.normal[0] = tempMesh.m_vertices[v].m_normal.x;
					vertex0.normal[1] = tempMesh.m_vertices[v].m_normal.y;
					vertex0.normal[2] = tempMesh.m_vertices[v].m_normal.z;

					if (tempMesh.m_vertices.at(0).m_textureCoords.x) // NEED TO CHECK THIS TO MAKE SURE
					{
						// has textures
						vertex0.uv[0] = tempMesh.m_vertices[v].m_textureCoords.x;
						vertex0.uv[1] = tempMesh.m_vertices[v].m_textureCoords.y;
					}
					else
					{
						vertex0.uv[0] = 1;
						vertex0.uv[1] = 1;
					}



					verticesPtr->push_back(vertex0);
				}

				int faceCount = tempMesh.m_indices.size();

				for (int f = 0; f < faceCount; f++)
				{
					indicesPtr->push_back(tempMesh.m_indices[f]);
				}

			}
		

	
	
	std::shared_ptr<ObjInstanceShape> bulletObj(new ObjInstanceShape);

	bulletObj->m_vertices = verticesPtr;
	bulletObj->m_numOfVertices = verticesPtr->size();
	bulletObj->m_indices = indicesPtr;
	bulletObj->m_numOfIndices = indicesPtr->size();
	UTIL::LOG(UTIL::LOG::INFO) << "Obj Loaded: number of extracted vertices :" << bulletObj->m_numOfVertices * 8;
	UTIL::LOG(UTIL::LOG::INFO) << "Obj Loaded: number of extracted indices :" << bulletObj->m_numOfIndices;
	for (int i = 0; i < 4; i++)
		bulletObj->m_scaling[i] = 1;

	return bulletObj;
}
