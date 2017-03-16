/**
* @Author	Luke Newell
* @date		18th Feburary 2017
* @brief    implemention of the model class
*/
#include "modelLoader.h"
#include <memory>

GRAPHICS::Model::Model(GLchar* path) {
	this->LoadModel(path);
}
GRAPHICS::Model::Model(const char* path) {
	this->LoadModel(path);
}

void GRAPHICS::Model::Render(std::shared_ptr<GRAPHICS::Shader> shader) {
	for (GLuint i = 0; i < this->m_meshes.size(); i++)
		this->m_meshes[i].Render(shader);
}

std::vector<GRAPHICS::Mesh> GRAPHICS::Model::GetMeshes()
{
	
	return m_meshes;
}

void GRAPHICS::Model::LoadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		UTIL::LOG(UTIL::LOG::FAULT) << "ERROR::ASSIMP::" << import.GetErrorString();
		return;
	}

	this->m_directory = path.substr(0, path.find_last_of('/'));

	this->ProcessNode(scene->mRootNode, scene);
}

void GRAPHICS::Model::ProcessNode(aiNode* node, const aiScene* scene) {
	// proces all of the node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_meshes.push_back(this->ProcessMesh(mesh, scene));
	}
	// process all of its childrens if it has any and do the same
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

GRAPHICS::Mesh GRAPHICS::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 vector1; // assimp uses its own vector class that doesnt convert to glm's vec3

		vector1.x = mesh->mVertices[i].x;
		vector1.y = mesh->mVertices[i].y;
		vector1.z = mesh->mVertices[i].z;
		vertex.m_position = vector1;

		vector1.x = mesh->mNormals[i].x;
		vector1.y = mesh->mNormals[i].y;
		vector1.z = mesh->mNormals[i].z;
		vertex.m_normal = vector1;

		if (mesh->mTextureCoords[0]) // if it has texture coords
		{
			glm::vec2 vector2;
			
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.m_textureCoords = vector2;
		}
		else
			vertex.m_textureCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// now to iterate through each of the mesh's faces and retrieve the indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		// retrieve all the indices of the face and store them in the idices vector
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		//diffuse
		std::vector<Texture> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//specular
		std::vector<Texture> specularMaps = this->LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//ambient
		std::vector<Texture> ambientMaps = this->LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
		textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<GRAPHICS::Texture> GRAPHICS::Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	GLuint t = 0;
	GLuint test = material->GetTextureCount(type);
	for ( t = 0; t < material->GetTextureCount(type); t++)
	{
		aiString str;
		material->GetTexture(type, t, &str);
		GLboolean skip = false;

		for (GLuint j = 0; j < m_texturesLoaded.size(); j++)
		{
			if (m_texturesLoaded[j].m_path == str)
			{
				textures.push_back(m_texturesLoaded[j]);
				skip = TRUE;
				break;
			}
		} // if the texture hasn't already been loaded in
		if (!skip)
		{
			Texture texture;
			texture.m_id = TextureFromFile(str.C_Str(), this->m_directory);
			texture.m_type = typeName;
			texture.m_path = str;
			textures.push_back(texture);
		}
	}
	return textures;
}

GLint GRAPHICS::TextureFromFile(const char * path, std::string directory)
{

	//generate texture ID and load Texture data
	std::string filename = std::string(path);

	filename = directory + '/' + filename;
	std::cout << filename << std::endl;
	GLuint textureID;

	gl::GenTextures(1, &textureID);
	int width, height;

	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	//Assign Texture to the ID

	gl::BindTexture(gl::TEXTURE_2D, textureID);
	gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGB, width, height, 0, gl::RGB, gl::UNSIGNED_BYTE, image);
	gl::GenerateMipmap(gl::TEXTURE_2D);

	// Parameters 
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::BindTexture(gl::TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}
