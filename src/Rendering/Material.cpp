#include "Material.h"
#include "const.h"

Material::Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	m_shader.setupShader(vertexShaderPath, fragmentShaderPath);
	m_texture.loadFromImage(TEXTURE::NO_TEX_PATH);
	m_colorUniformLocation = m_shader.getLocation(SHADER::COLOR);
	m_shader.setVec4(m_colorUniformLocation, m_color);
	m_modelViewProjLocation = m_shader.getLocation(SHADER::MODEL_VIEW_PROJ);
	m_tilingLocation = m_shader.getLocation(SHADER::TILING);
	m_shader.setFloat(m_tilingLocation, 1);
}

Material::Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& textureImagePath)
{
	m_shader.setupShader(vertexShaderPath, fragmentShaderPath);
	m_texture.loadFromImage(textureImagePath);
	m_colorUniformLocation = m_shader.getLocation(SHADER::COLOR);
	m_shader.setVec4(m_colorUniformLocation, m_color);
	m_modelViewProjLocation = m_shader.getLocation(SHADER::MODEL_VIEW_PROJ);
	m_tilingLocation = m_shader.getLocation(SHADER::TILING);
	m_shader.setFloat(m_tilingLocation, 1);
}

void Material::setModelViewProjMatrix(const glm::mat4& modelViewProj)
{
	m_shader.setMat4(m_modelViewProjLocation, modelViewProj);
}
