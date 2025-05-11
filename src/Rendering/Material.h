#pragma once
#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& textureImagePath);

	void setModelViewProjMatrix(const glm::mat4& modelViewProj);

	inline glm::vec4 getColor() const { return m_color; }
	inline Shader& getShader() { return m_shader; }
	inline Texture& getTexture() { return m_texture; }
	

	inline void setColor(glm::vec4 color)
	{
		m_color = color;
		m_shader.setVec4(m_colorUniformLocation, m_color);
	}

	void activate() const
	{
		m_shader.activate();
		m_texture.activate();
	}
private:
	//material owns shader and texture
	Texture m_texture;
	glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Shader m_shader;
	GLint m_colorUniformLocation = -1;
	GLint m_hasTextureUniformLocation = -1;
	GLint m_modelViewProjLocation = -1;
};
