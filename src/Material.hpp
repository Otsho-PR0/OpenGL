#pragma once

#include <glm/glm.hpp>
#include "Texture.hpp"

class Material
{
public:
	Material();
	Material(const char* name, const char* diffPath, const char* roughPath);

	void init(const char* name, const char* diffPath, const char* roughPath);

	std::string name;

	void use();

private:
	Texture diff;
	Texture rough;
};
