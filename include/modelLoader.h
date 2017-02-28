/**
* @Author	Luke Newell
* @date		16th Feburary 2017
* @brief    Class to load external models
*/

#pragma once


#include <iostream>

#include <gl.hpp>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL2/SOIL2.h>

#include "shader.h"
#include "meshLoader.h"
#include "logger.h"

namespace GRAPHICS {
	/*!
	\brief Constructor for the Mesh Class
	\param vertices are the given vertices for the current mesh
	\param indices are the given indices of the current mesh
	\param textures are the given texture for the current mesh
	*/
	GLint TextureFromFile(const char* path, std::string directory);

	/*! \class Model
	\brief Contains the data to pass to each of its mesh's and Textures
	*/
	class Model {
	public:
		/*!
		\brief Constructor for the Model Class
		\param path takes the path of the model e.g. xx/yy.obj
		*/
		Model(GLchar* path);

		/*!
		\brief Render's each Node to draw the Model
		\param shader is the current shader program in use
		*/
		void Render(Shader* shader);
	private:
		/*!
		\brief Loads the data into our class
		\param path is the directory where we acquire the data
		*/
		void LoadModel(std::string path);
		/*!
		\brief Processes the Nodes, Nodes contain a group of Meshs
		\param node, takes the address to the node which contains the Meshs
		\param scene takes the data for the material
		*/
		void ProcessNode(aiNode* node, const aiScene* scene);
		/*!
		\brief Process the Meshes within a node
		\param Mesh takes the addreess to a given mesh to process its data
		\param Scene is the address of the scene given from the ProcessNode
		*/
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		/*!
		\brief LoadMaterialTextures load the material within the object data to determine its lighting
		\param material is the type of material the current mesh is.
		\param type is the type of texture it is
		\param typename is the name of the type that it is
		*/
		std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

		std::vector<Mesh> m_meshes; //!< a vector of meshes for the nodes to process
		std::string m_directory; //!< directory where the data is stored
		std::vector<Texture> m_texturesLoaded; //!<vector of the textures loaded
	};
}