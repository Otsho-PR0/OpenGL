#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position);

	glm::mat4 getViewMatrix();

	glm::vec3 position;
	glm::vec3 direction;
};
