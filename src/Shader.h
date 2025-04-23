#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>


class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	inline GLuint getID() const { return m_shaderProgramID; }

	inline GLint getLocation(const std::string& name) const { return glGetUniformLocation(m_shaderProgramID, name.c_str()); }

	void activate();
	void setBool(GLint location, bool value);
	void setInt(GLint location, int value);
	void setFloat(GLint location, float value);
	void setMat4(GLint location, glm::mat4 mat4);
private:
	GLuint m_shaderProgramID = 0u;
private:
	std::string readFromFile(const std::string& filePath);
};

