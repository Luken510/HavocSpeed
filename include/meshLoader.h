/**
* @Author	Luke Newell
* @date    15th Feburary 2017
* @brief    Loader for the meshes within a model
*/

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <memory>

#include <glm/glm.hpp>
#include <gl.hpp>
#include <assimp/scene.h>

#include "shader.h"

namespace GRAPHICS {
	/*! \struct Vertex
	\brief To store the models Position, position normal, and Texture co-ords.
	*/
	struct Vertex {
		glm::vec3 m_position; //!< Position of the current vertex for the mesh
		glm::vec3 m_normal; //!< Position of the normal of the current vertex for the mesh
		glm::vec2 m_textureCoords; //!< position of the texture co-ords for the mesh
	};
	/*! \struct Texture
	\brief Stores the Texture's ID, type and Path to the file.
	*/
	struct Texture {
		GLuint m_id; //!< the ID of the texture, used to gain access to it
		std::string m_type; //!< the type of data it is, e.g. XML
		aiString m_path;  //!< Directory to the texture image
	};

	/*! \class Mesh
	\brief Mesh class, contains the data needed for each part of the model
	*/
	class Mesh {
	public:
		/*!
		\brief Constructor for the Mesh Class
		\param vertices are the given vertices for the current mesh
		\param indices are the given indices of the current mesh
		\param textures are the given texture for the current mesh
		*/
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

		/*!
		\brief Render function binds the mesh data to the shader program
		\param shader takes a pointer to the shader program
		*/
		void Render(std::shared_ptr<Shader> shader);

		std::vector<Vertex> m_vertices; //!< a vector array of the Struct Vertex for the mesh vertices
		std::vector<GLuint> m_indices; //!< a vector array of GL unsigned ints, used for the mesh indices 
		std::vector<Texture> m_textures; //!< a vector array of the Struct Texture, used for the Textures for the mesh
	private:
		/*!
		\brief bings the VAO, VBO, and EBO to the shader program
		*/
		void SetupMesh();

		GLuint VAO, VBO, EBO; //!< creates the holders for the VAO, VBO and EBO bindings for the shader program
	};
}