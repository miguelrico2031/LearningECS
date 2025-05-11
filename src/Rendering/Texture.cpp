#include "Texture.h"
#include "Log.h"
#include "stb_image.h"


Texture::Texture()
{
}

Texture::Texture(const std::string& imagePath)
{
	loadFromImage(imagePath);
}

Texture::~Texture()
{
	clear();
}

void Texture::setupTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// set texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::loadFromImage(const std::string& imagePath)
{
	if (m_textureID == 0u)
	{
		setupTexture();
	}

	stbi_set_flip_vertically_on_load(true);
	int nrChannels;
	unsigned char* data = stbi_load(imagePath.c_str(), &m_width, &m_height, &nrChannels, STBI_rgb_alpha);

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG_ERROR("Failed to load texture: " << imagePath);
	}
	stbi_image_free(data);
}

void Texture::activate() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}
void Texture::clear()
{
	if (m_textureID != 0u)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
		m_width = m_height = -1;
	}
}
