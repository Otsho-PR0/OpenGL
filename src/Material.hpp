#pragma once

#include <glm/glm.hpp>
#include "Texture.hpp"

class Material
{
public:
	Material();
	Material(const char* name, const char* diffPath, const char* roughPath, const char* normPath);

	void init(const char* name, const char* diffPath, const char* roughPath, const char* normPath);

	std::string name;

	void use();

private:
	Texture diff;
	Texture rough;
	Texture norm;
};
