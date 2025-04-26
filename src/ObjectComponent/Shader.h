#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

namespace ObjectComponent
{
	class Shader
	{
	public:
		Shader() = default;
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		inline GLuint getID() const { return m_shaderProgramID; }

		inline GLint getLocation(const std::string& name) const { return glGetUniformLocation(m_shaderProgramID, name.c_str()); }

		void setupShader(const std::string& vertexPath, const std::string& fragmentPath);

		void activate() const;
		void setBool(GLint location, bool value);
		void setInt(GLint location, int value);
		void setFloat(GLint location, float value);
		void setVec3(GLint location, glm::vec3 vec3);
		void setVec4(GLint location, glm::vec4 vec4);
		void setMat4(GLint location, glm::mat4 mat4);

		void clear();
	private:
		GLuint m_shaderProgramID = 0u;
	private:
		std::string readFromFile(const std::string& filePath);
	};
}