#pragma once

#include <vec3.hpp>
#include <vec2.hpp>

#include "Material.hpp"
#include "Transform.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh&) = delete;
	Mesh(Material*, Vertex*, size_t, unsigned int*, size_t);

	void Draw(Shader&);

	static Mesh* cube();
	static Mesh* plane(float, float);

	Transform transform;
	Material* material;

private:
	unsigned int m_ID;
	unsigned int count;
};
