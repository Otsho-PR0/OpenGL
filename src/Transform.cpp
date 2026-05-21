#include <Transform.hpp>
#include <ext/matrix_transform.hpp>

glm::mat4 Transform::GetModel()
{
	return glm::translate({1.0f}, position) * glm::rotate({1.0f}, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate({1.0f}, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate({1.0f}, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale({1.0f}, scale);
}

glm::mat3 Transform::GetBasis()
{
	return glm::rotate({1.0f}, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate({1.0f}, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate({1.0f}, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale({1.0f}, scale);
}

glm::mat4 Transform::GetLookAt()
{
	return glm::lookAt(position, position + GetBasis() * glm::vec3(0.0f, 0.0f, -1.0f), GetBasis() * glm::vec3(0.0f, 1.0f, 0.0f));
}