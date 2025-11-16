#include "Texture.hpp"

Texture::Texture()
{}

Texture::Texture(const char* texturePath) :
	name(texturePath)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint width, height, channels;

	GLubyte* data = stbi_load(texturePath, &width, &height, &channels, 0);

	GLint format = (channels == 1) ? GL_RED : (channels == 2) ? GL_RG : (channels == 3) ? GL_RGB : (channels == 4) ? GL_RGBA : 0;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

void Texture::init(const char* texturePath)
{
	name = texturePath;
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint width, height, channels;

	GLubyte* data = stbi_load(texturePath, &width, &height, &channels, 0);

	GLint format = (channels < 4) ? (channels < 3) ? (channels < 2) ? (channels < 1) ? : GL_RG : GL_RG : GL_RGB : GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

GLuint Texture::getID()
{
	return m_ID;
}

void Texture::bind(GLint texture)
{
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}
