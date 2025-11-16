#pragma once

#include <GL/glew.h>

#include <stb_image.h>
#include <string>

class Texture
{
public:
	Texture();
	Texture(const char* texturePath);

	void init(const char* texturePath);

	GLuint getID();
	void bind(GLint texture = GL_TEXTURE0);

	std::string name;

private:
	GLuint m_ID;
};
