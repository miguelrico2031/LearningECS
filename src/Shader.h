#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	inline GLuint getID() const { return m_shaderProgramID; }

	// use this to cache the location instead of fetching it every setBool/Int/Float call
	inline GLint getLocation(const std::string& name) const { return glGetUniformLocation(m_shaderProgramID, name.c_str()); }

	void activate();
	void setBool(const std::string& name, bool value);
	void setBool(GLint location, bool value);
	void setInt(const std::string& name, int value);
	void setInt(GLint location, int value);
	void setFloat(const std::string& name, float value);
	void setFloat(GLint location, float value);
private:
	GLuint m_shaderProgramID = 0u;
private:
	std::string readFromFile(const std::string& filePath);
};

