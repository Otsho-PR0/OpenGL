#include "Camera.hpp"

Camera::Camera() :
	position(0.0f),
	direction(0.0f, 0.0f, -1.0f)
{}

Camera::Camera(glm::vec3 position) :
	position(position),
	direction(0.0f, 0.0f, -1.0f)
{}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position+direction, glm::vec3(0.0f, 1.0f, 0.0f));
}
