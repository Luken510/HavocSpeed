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


				int faceCount = tempMesh.m_indices.size();

				for (int f = 0; f < faceCount; f+=3)
				{
					
					int vtxBaseIndex = tempMesh.m_vertices.size();

					if (f<0 && f>-(int)(tempMesh.m_indices.size()))
					{
						continue;
					}
					
					ObjInstanceVertex vertex0;
					vertex0.xyz[0] = tempMesh.m_vertices[f].m_position.x;
					vertex0.xyz[1] = tempMesh.m_vertices[f].m_position.y;
					vertex0.xyz[2] = tempMesh.m_vertices[f].m_position.z;
					//vertex0.xyzw[3] = 0.f;

					vertex0.normal[0] = tempMesh.m_vertices[f].m_normal.x;
					vertex0.normal[1] = tempMesh.m_vertices[f].m_normal.y;
					vertex0.normal[2] = tempMesh.m_vertices[f].m_normal.z;

					if (tempMesh.m_vertices.at(0).m_textureCoords.x) // NEED TO CHECK THIS TO MAKE SURE
					{
						// has textures
						vertex0.uv[0] = tempMesh.m_vertices[f].m_textureCoords.x;
						vertex0.uv[1] = tempMesh.m_vertices[f].m_textureCoords.y;
					}
					else
					{
						vertex0.uv[0] = 0;
						vertex0.uv[1] = 0;
					}
					
					ObjInstanceVertex vertex1;
					vertex1.xyz[0] = tempMesh.m_vertices[f].m_position.x;
					vertex1.xyz[1] = tempMesh.m_vertices[f].m_position.y;
					vertex1.xyz[2] = tempMesh.m_vertices[f].m_position.z;
					//vertex0.xyzw[3] = 0.f;

					vertex1.normal[0] = tempMesh.m_vertices[f].m_normal.x;
					vertex1.normal[1] = tempMesh.m_vertices[f].m_normal.y;
					vertex1.normal[2] = tempMesh.m_vertices[f].m_normal.z;

					if (tempMesh.m_vertices.at(0).m_textureCoords.x) // NEED TO CHECK THIS TO MAKE SURE
					{
						// has textures
						vertex1.uv[0] = tempMesh.m_vertices[f].m_textureCoords.x;
						vertex1.uv[1] = tempMesh.m_vertices[f].m_textureCoords.y;
					}
					else
					{
						vertex1.uv[0] = 0;
						vertex1.uv[1] = 0;
					}

					ObjInstanceVertex vertex2;
					vertex2.xyz[0] = tempMesh.m_vertices[f].m_position.x;
					vertex2.xyz[1] = tempMesh.m_vertices[f].m_position.y;
					vertex2.xyz[2] = tempMesh.m_vertices[f].m_position.z;
					//vertex0.xyzw[3] = 0.f;

					vertex2.normal[0] = tempMesh.m_vertices[f].m_normal.x;
					vertex2.normal[1] = tempMesh.m_vertices[f].m_normal.y;
					vertex2.normal[2] = tempMesh.m_vertices[f].m_normal.z;

					if (tempMesh.m_vertices.at(0).m_textureCoords.x) // NEED TO CHECK THIS TO MAKE SURE
					{
						// has textures
						vertex2.uv[0] = tempMesh.m_vertices[f].m_textureCoords.x;
						vertex2.uv[1] = tempMesh.m_vertices[f].m_textureCoords.y;
					}
					else
					{
						vertex2.uv[0] = 0;
						vertex2.uv[1] = 0;
					}

					verticesPtr->push_back(vertex0);
					verticesPtr->push_back(vertex1);
					verticesPtr->push_back(vertex2);
					indicesPtr->push_back(vtxBaseIndex);
					indicesPtr->push_back(vtxBaseIndex+1);
					indicesPtr->push_back(vtxBaseIndex+2);

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
