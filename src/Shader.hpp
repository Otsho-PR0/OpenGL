#pragma once

#include <GL/glew.h>

class Shader
{
public:
	Shader(const char* vsPath, const char* fsPath);

	void use();

	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setVec3(const char* name, float* value);
	void setVec4(const char* name, float* value);
	void setMat3(const char* name, float* value);
	void setMat4(const char* name, float* value);

private:
	GLuint m_ID;
};
