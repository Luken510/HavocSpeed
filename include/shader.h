/**
* @Author	Luke Newell
* @date     15th February 2017
* @brief    Shader loader.
*/

#pragma once
#pragma warning( disable : 4290 )  

#include <map>
#include <iostream>
#include <string>
#include <stdexcept>

#include <gl.hpp>
#include <glm/glm.hpp>

class ShaderException : public std::runtime_error {
public:
	/*!
	\brief Constructor.
	\param msg String containing the error message.
	*/
	ShaderException(const std::string& msg) :
		std::runtime_error(msg) { }
};

namespace ShaderTypes {
	/*!
	\brief Types of shaders.
	*/
	enum ShaderType {
		VERTEX = gl::VERTEX_SHADER,
		FRAGMENT = gl::FRAGMENT_SHADER,
		GEOMETRY = gl::GEOMETRY_SHADER,
		TESS_CONTROL = gl::TESS_CONTROL_SHADER,
		TESS_EVALUATION = gl::TESS_EVALUATION_SHADER,
		COMPUTE = gl::COMPUTE_SHADER
	};
};
namespace GRAPHICS {
	class Shader {
	public:
		/*!
		\brief Constructor.
		*/
		Shader();

		/*!
		\brief Destructor.
		*/
		~Shader();

		/*!
		brief Compile and link the shader to the program.
		param vertexshader path to the vs file for vertex shading
		param fragmentshader path to the vs file for the fragment shading
		*/
		void CompileAndLinkShader(std::string vertexshader, std::string fragmentshader);

		/*!
		\brief Compile shader.
		\param filename Constant char containing the filename.
		*/
		void CompileShader(const char* filename) throw (ShaderException);

		/*!
		\brief Compile shader.
		\param filename Constant char containing the filename.
		\param type Shader type enum.
		*/
		void CompileShader(const char* filename, ShaderTypes::ShaderType type) throw (ShaderException);

		/*!
		\brief Compile shader.
		\param source Constant string containing the source.
		\param type Shader type enum.
		\param filename Constant char containing the filename.
		*/
		void CompileShader(const std::string& source, ShaderTypes::ShaderType type,
			const char* filename = NULL) throw (ShaderException);

		/*!
		\brief Destructor.
		*/
		void Link() throw (ShaderException);

		/*!
		\brief Destructor.
		*/
		void Validate() throw (ShaderException);

		/*!
		\brief Destructor.
		*/
		void Use() throw (ShaderException);


		/*!
		\brief Get handle.
		*/
		int GetHandle();

		/*!
		\brief Is linked.
		*/
		bool IsLinked();


		/*!
		\brief Binds attribute location.
		\param source Constant string containing the source.
		\param type Shader type enum.
		\param filename Constant char containing the filename.
		*/
		void BindAttributeLocation(GLuint location, const char* name);

		/*!
		\brief Binds fragment location.
		\param source Constant string containing the source.
		\param type Shader type enum.
		\param filename Constant char containing the filename.
		*/
		void BindFragmentDataLocation(GLuint location, const char* name);


		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param x Float containing the positional value of x.
		\param y Float containing the positional value of y.
		\param z Float containing the positional value of z.
		*/
		void SetUniform(const char* name, float x, float y, float z);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param v Constant 2D vector value.
		*/
		void SetUniform(const char* name, const glm::vec2& v);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param v Constant 3D vector value.
		*/
		void SetUniform(const char* name, const glm::vec3& v);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param v Constant 4D vector value.
		*/
		void SetUniform(const char* name, const glm::vec4& v);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param m Constant 3D matrix value.
		*/
		void SetUniform(const char* name, const glm::mat3& m);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param m Constant 4D matrix value.
		*/
		void SetUniform(const char* name, const glm::mat4& m);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param val Integer type value.
		*/
		void SetUniform(const char* name, int val);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param val Float type value.
		*/
		void SetUniform(const char* name, float val);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param val Bool type value.
		*/
		void SetUniform(const char* name, bool val);

		/*!
		\brief Sets uniform.
		\param name Constant char containing the name.
		\param val GL unsigned integer value.
		*/
		void SetUniform(const char* name, GLuint val);


		/*!
		\brief Prints active uniforms.
		*/
		void PrintActiveUniforms();

		/*!
		\brief Prints active uniform blocks.
		*/
		void PrintActiveUniformBlocks();

		/*!
		\brief Prints active attributes.
		*/
		void PrintActiveAttributes();


		/*!
		\brief Gets the type string.
		\param type GL enum type.
		*/
		const char* GetTypeString(GLenum type);
		/*!
		\brief Shader.
		\param other Constant shader type value.
		*/
		Shader(const Shader& other) { }
		/*!
		\brief Operator.
		\param other Constant shader type value.
		*/
		Shader& operator = (const Shader& other) { return*this; }

	private:
		/*!
		\brief Gets the uniform location.
		\param name Constant char containing the name.
		*/
		GLint getUniformLocation(const char* name);

		/*!
		\brief A bool value of whether a file exists or not.
		\param filename Constant string containing the filename.
		*/
		bool fileExists(const std::string& filename);

		/*!
		\brief The string value of the extension to get.
		\param filename Constant char containing the filename.
		*/
		std::string getExtension(const char* filename);

		

		


		int m_handle;  //!< Integer value for shader handle.
		bool m_linked; //!< Bool value for if the shaders linked correctly.
		std::map<std::string, int> m_uniformLocation; //!< Location of all uniform variables.
	};
}