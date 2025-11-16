#include "Shader.hpp"

#include <fstream>

void getInfoLog(GLuint shader)
{
	int success;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* infoLog = new char[length];
		glGetShaderInfoLog(shader, length, nullptr, infoLog);
		printf("%s", infoLog);
		delete[] infoLog;
	}
}

Shader::Shader(const char* vsPath, const char* fsPath)
{
	std::string line;

	std::ifstream vsFile(vsPath);
	std::string vs;
	while (std::getline(vsFile, line))
		vs.append(line).append("\n");
	const char* vertexSource = vs.c_str();
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);
	getInfoLog(vertex);

	std::ifstream fsFile(fsPath);
	std::string fs;
	while (std::getline(fsFile, line))
		fs.append(line).append("\n");
	const char* fragmentSource = fs.c_str();
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, nullptr);
	glCompileShader(fragment);
	getInfoLog(fragment);

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
}

void Shader::use()
{
	glUseProgram(m_ID);
}

void Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(m_ID, name), value);
}

void Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(m_ID, name), value);
}

void Shader::setVec3(const char* name, float* value)
{
	glUniform3fv(glGetUniformLocation(m_ID, name), 1, value);
}

void Shader::setVec4(const char* name, float* value)
{
	glUniform4fv(glGetUniformLocation(m_ID, name), 1, value);
}

void Shader::setMat3(const char* name, float* value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value);
}

void Shader::setMat4(const char* name, float* value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, value);
}
