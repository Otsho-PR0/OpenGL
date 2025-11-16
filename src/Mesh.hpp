#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Material.hpp"

class Mesh
{
public:
	Mesh();
	Mesh(GLfloat* vertices, size_t verticesSize, GLushort* indices, size_t indicesSize, Material material);

	void init(GLfloat* vertices, size_t verticesSize, GLushort* indices, size_t indicesSize, Material material);
	void draw();

	static Mesh box(float width, float height, float shrek);
	static Mesh plane(float width, float height);

private:
	GLfloat* m_Vertices;
	size_t m_VerticesSize;
	GLushort* m_Indices;
	size_t m_IndicesSize;
	Material m_Material;

	GLuint VAO, VBO, EBO;
};
