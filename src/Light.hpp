#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	enum Type
	{
		Point,
		Directional,
		Spot
	};

public:
	Light(int type);

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

private:
	int type;
};
