#include "Texture.h"
#include "Log.h"
#include "stb_image.h"

Texture::Texture()
{
	generateTexture();
}

Texture::Texture(const std::string& imagePath)
{
	generateTexture();
	loadFromImage(imagePath);
}

void Texture::generateTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// set texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // set texture filtering to GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::loadFromImage(const std::string& imagePath)
{
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

void Texture::activate()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}
