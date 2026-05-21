#include <Shader.hpp>

#include <glad/gl.h>
#include <fstream>
#include <string>

void CheckCompilationErrors(GLuint shader)
{
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, &length, log);

		printf("%s", log);
		delete[] log;
	}
}

void CheckLinkErrors(GLuint program)
{
	GLint status;

	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (!status)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(program, length, &length, log);

		printf("%s", log);
		delete[] log;
	}
}

Shader::Shader(const char* vShader, const char* fShader)
{
	std::string line;

	std::fstream vertex(vShader);
	if (!vertex)
		printf("Failed to load Vertex Shader '%s'\n", vShader);
	std::string vSource;
	while (std::getline(vertex, line))
		vSource += line + "\n";

	const char* vertexShader = vSource.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShader, nullptr);
	glCompileShader(vs);
	CheckCompilationErrors(vs);

	std::fstream fragment(fShader);
	if (!fragment)
		printf("Failed to load Fragment Shader '%s'\n", fShader);
	std::string fSource;
	while (std::getline(fragment, line))
		fSource += line + "\n";

	const char* fragmentShader = fSource.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShader, nullptr);
	glCompileShader(fs);
	CheckCompilationErrors(fs);

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vs);
	glAttachShader(m_ID, fs);
	glLinkProgram(m_ID);
	CheckLinkErrors(m_ID);
}

void Shader::Bind()
{
	glUseProgram(m_ID);
}

void Shader::SetInt(const char* n, int v)
{
	glUniform1i(glGetUniformLocation(m_ID, n), v);
}

void Shader::SetFloat(const char* n, float v)
{
	glUniform1f(glGetUniformLocation(m_ID, n), v);
}

void Shader::SetVec3(const char* n, float* v)
{
	glUniform3fv(glGetUniformLocation(m_ID, n), 1, v);
}

void Shader::SetMat4(const char* n, float* v)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, n), 1, GL_FALSE, v);
}
