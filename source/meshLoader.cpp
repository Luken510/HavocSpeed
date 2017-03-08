/**
* @Author	Luke Newell
* @date     16th Feburary 2017
* @brief    implementation of the Mesh class
*/
#include "meshLoader.h"
#include <memory>

GRAPHICS::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_textures = textures;
	this->SetupMesh();
}

void GRAPHICS::Mesh::SetupMesh()
{
	gl::GenVertexArrays(1, &this->VAO);
	gl::GenBuffers(1, &this->VBO);
	gl::GenBuffers(1, &this->EBO);

	gl::BindVertexArray(this->VAO);

	gl::BindBuffer(gl::ARRAY_BUFFER, this->VBO); // setup data VBO
	gl::BufferData(gl::ARRAY_BUFFER, this->m_vertices.size() * sizeof(Vertex), &this->m_vertices[0], gl::STATIC_DRAW);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, this->EBO); // set up element VBO
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(GLuint), &this->m_indices[0], gl::STATIC_DRAW);

	// Vertex Positions
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, sizeof(Vertex), (GLvoid*)0);

	// Vertex Normals
	gl::EnableVertexAttribArray(1);
	gl::VertexAttribPointer(1, 3, gl::FLOAT, FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_normal));

	// Vertex Texture Co-ords
	gl::EnableVertexAttribArray(2);
	gl::VertexAttribPointer(2, 2, gl::FLOAT, FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_textureCoords));

	gl::BindVertexArray(0);

}

void GRAPHICS::Mesh::Render(std::shared_ptr<GRAPHICS::Shader> shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;


	for (GLuint i = 0; i < (this->m_textures.size()); i++)
	{
		gl::ActiveTexture(gl::TEXTURE0 + i); // activating the proper texture unit before binding the data											 

		std::string Name = this->m_textures[i].m_type;
		std::string Number = (Name == "texture_diffuse") ? std::to_string(diffuseNr++) : std::to_string(specularNr);

		gl::Uniform1f(gl::GetUniformLocation(shader->GetHandle(), ("material." + Name + Number).c_str()), i); // now finds the correct location in the shader using the above gluint to string function
		gl::BindTexture(gl::TEXTURE_2D, this->m_textures[i].m_id);
	}

	gl::ActiveTexture(gl::TEXTURE0);

	gl::BindVertexArray(this->VAO);
	gl::DrawElements(gl::TRIANGLES, this->m_indices.size(), gl::UNSIGNED_INT, 0);
	gl::BindVertexArray(0);

}

