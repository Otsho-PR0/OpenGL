#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include <vec3.hpp>

class Material
{
public:

	void Bind(Shader&);

	Texture tDiffuse;
	Texture tAO;
	Texture tNormals;
	Texture tRoughness;
	Texture tMetalness;
	Texture tEmissive;

	glm::vec3 albedo;
	float ao;
	float roughness;
	float metalness;
};
