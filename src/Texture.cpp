#include <Texture.hpp>

#include <iostream>

#include <glad/gl.h>
#include <stb_image.h>

Texture::Texture(const char* path)
{
	int width, height, channels;
	GLubyte* data = stbi_load(path, &width, &height, &channels, 0);

	if (!data)
	{
		std::cerr << "Failed to open file: " << path << std::endl;
		return;
	}

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_SRGB8_ALPHA8 : GL_SRGB8, width, height, 0, channels == 4 ? GL_RGBA :  GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind(GLenum texture)
{
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

unsigned int Texture::LoadTexture(const char* path, GLenum target, GLint internalFormat, GLenum format)
{
	int width, height, channels;
	GLubyte* data = stbi_load(path, &width, &height, &channels, 0);

	if (!data)
	{
		std::cerr << "Failed to open file: " << path << std::endl;
		return 0;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(target, texture);
	glTexImage2D(target, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void Texture::GiveID(unsigned int id)
{
	m_ID = id;
}