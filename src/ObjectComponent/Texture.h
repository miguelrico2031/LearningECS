#pragma once
#include <glad/glad.h>
#include <string>

namespace ObjectComponent
{
	class Texture
	{
	public:
		// creates empty texture
		Texture();
		//creates texture from an image
		Texture(const std::string& imagePath);

		~Texture();

		inline GLuint getID() const { return m_textureID; }
		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }

		void loadFromImage(const std::string& imagePath);

		void activate() const;

		void clear();

	private:
		GLuint m_textureID = 0u;
		int m_width = -1, m_height = -1;
	private:
		void setupTexture();
	};
}
