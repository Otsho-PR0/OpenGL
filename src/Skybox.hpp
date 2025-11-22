#pragma once

#include <GL/glew.h>

#include "Shader.hpp"
#include <glm/glm.hpp>

#include <string>
#include <vector>

class Skybox
{
public:
	Skybox(std::vector<std::string> faces, glm::mat4 projection);

	void draw(glm::mat4 view);

private:
	Shader m_Shader;
	GLuint m_ID;
	GLuint m_VAO;
};
