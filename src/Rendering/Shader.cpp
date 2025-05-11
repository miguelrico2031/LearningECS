#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Log.h"
#include <glm/gtc/type_ptr.hpp>



Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	setupShader(vertexPath, fragmentPath);
}

Shader::~Shader()
{
	clear();
}

void Shader::activate() const
{
	glUseProgram(m_shaderProgramID);
}


void Shader::setBool(GLint location, bool value)
{
	glUseProgram(m_shaderProgramID);
	glUniform1i(location, (int)value);
}


void Shader::setInt(GLint location, int value)
{
	glUseProgram(m_shaderProgramID);
	glUniform1i(location, value);
}


void Shader::setFloat(GLint location, float value)
{
	glUseProgram(m_shaderProgramID);
	glUniform1f(location, value);
}

void Shader::setVec3(GLint location, glm::vec3 vec3)
{
	glUseProgram(m_shaderProgramID);
	glUniform3fv(location, 1, glm::value_ptr(vec3));
}

void Shader::setVec4(GLint location, glm::vec4 vec4)
{
	glUseProgram(m_shaderProgramID);
	glUniform4fv(location, 1, glm::value_ptr(vec4));
}

void Shader::setMat4(GLint location, glm::mat4 mat4)
{
	glUseProgram(m_shaderProgramID);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::clear()
{
	if (m_shaderProgramID != 0u)
	{
		glDeleteProgram(m_shaderProgramID);
		m_shaderProgramID = 0;
	}
}

void Shader::setupShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexShaderSourceStr = readFromFile(vertexPath);
	const char* vertexShaderSource = vertexShaderSourceStr.c_str();

	std::string fragmentShaderSourceStr = readFromFile(fragmentPath);
	const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

	//vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		LOG_ERROR("Error: vertex shader compilation failed: \n" << errorLog);
		return;
	}


	//fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		LOG_ERROR("Error: fragment shader compilation failed: \n" << errorLog);
		return;
	}

	//shader program
	m_shaderProgramID = glCreateProgram();

	glAttachShader(m_shaderProgramID, vertexShader);
	glAttachShader(m_shaderProgramID, fragmentShader);
	glLinkProgram(m_shaderProgramID);

	glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgramID, 512, NULL, errorLog);
		LOG_ERROR("Error: shader program linking failed: \n" << errorLog);
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::readFromFile(const std::string& filePath)
{
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit); //ensure that can throw exceptions
	try
	{
		file.open(filePath);
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	catch (std::ifstream::failure error)
	{
		LOG_ERROR("File not succesfully read: " << filePath);
		return std::string();
	}
}

