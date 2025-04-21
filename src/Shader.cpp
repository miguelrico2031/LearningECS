#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Log.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexShaderSourceStr = readFromFile("../assets/shaders/basic.vert");
	const char* vertexShaderSource = vertexShaderSourceStr.c_str();

	std::string fragmentShaderSourceStr = readFromFile("../assets/shaders/basic.frag");
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

void Shader::activate()
{
	glUseProgram(m_shaderProgramID);
}

void Shader::setBool(const std::string& name, bool value)
{
	GLint location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	glUniform1i(location, (int)value);
}

void Shader::setBool(GLint location, bool value)
{
	glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	glUniform1i(location, value);
}

void Shader::setInt(GLint location, int value)
{
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_shaderProgramID, name.c_str());
	glUniform1f(location, value);
}

void Shader::setFloat(GLint location, float value)
{
	glUniform1f(location, value);
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
		LOG_ERROR("File not succesfully read.");
		return std::string();
	}
}

