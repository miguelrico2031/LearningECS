#pragma once
#include "Shader.h"
#include "Texture.h"

namespace ObjectComponent
{
	class Material
	{
	public:
		Material() = default;
		Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& textureImagePath);

		inline glm::vec4 getColor() const { return m_color; }
		inline Shader& getShader() { return m_shader; }
		inline Texture& getTexture() { return m_texture; }

		inline void setColor(glm::vec4 color) { m_color = color; }

		void activate() const
		{
			m_shader.activate();
			m_texture.activate();
		}
	private:
		//material owns shader and texture
		Shader m_shader;
		Texture m_texture;
		glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };

	};
}