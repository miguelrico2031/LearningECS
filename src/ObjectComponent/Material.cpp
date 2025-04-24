#include "Material.h"

namespace ObjectComponent
{
	Material::Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		m_shader.setupShader(vertexShaderPath, fragmentShaderPath);
	}

	Material::Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& textureImagePath)
	{
		m_shader.setupShader(vertexShaderPath, fragmentShaderPath);
		m_texture.loadFromImage(textureImagePath);
	}
}