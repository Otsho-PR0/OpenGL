#include "Mesh.hpp"
#include <cstdio>
#include <cstring>

Mesh::Mesh()
{}

Mesh::Mesh(GLfloat* vertices, size_t verticesSize, GLushort* indices, size_t indicesSize, Material material) :
	m_Vertices(new GLfloat[verticesSize / sizeof(GLfloat)]),
	m_VerticesSize(verticesSize),
	m_Indices(new GLushort[indicesSize / sizeof(GLushort)]),
	m_IndicesSize(indicesSize),
	m_Material(material)
{
	memcpy(m_Vertices, vertices, verticesSize);
	memcpy(m_Indices, indices, indicesSize);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::init(GLfloat* vertices, size_t verticesSize, GLushort* indices, size_t indicesSize, Material material)
{
	m_Vertices = new GLfloat[verticesSize / sizeof(GLfloat)];
	m_VerticesSize = verticesSize;
	m_Indices = new GLushort[indicesSize / sizeof(GLushort)];
	m_IndicesSize = indicesSize;
	m_Material = material;
	memcpy(m_Vertices, vertices, verticesSize);
	memcpy(m_Indices, indices, indicesSize);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw()
{
	m_Material.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_IndicesSize / sizeof(GLushort), GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

// Mesh Mesh::box(float width, float height, float shrek)
// {
// 	GLfloat vertices[] =
// 	{
// 		// position                    // normal             // texcoord
// 		// Front face
// 		-1.0f * width, -1.0f * height,  1.0f * shrek,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
// 		 1.0f * width, -1.0f * height,  1.0f * shrek,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
// 		 1.0f * width,  1.0f * height,  1.0f * shrek,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
// 		-1.0f * width,  1.0f * height,  1.0f * shrek,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
//
// 		// Back face
// 		-1.0f * width, -1.0f * height, -1.0f * shrek,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
// 		 1.0f * width, -1.0f * height, -1.0f * shrek,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
// 		 1.0f * width,  1.0f * height, -1.0f * shrek,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
// 		-1.0f * width,  1.0f * height, -1.0f * shrek,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//
// 		// Left face
// 		-1.0f * width, -1.0f * height, -1.0f * shrek, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
// 		-1.0f * width, -1.0f * height,  1.0f * shrek, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
// 		-1.0f * width,  1.0f * height,  1.0f * shrek, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
// 		-1.0f * width,  1.0f * height, -1.0f * shrek, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//
// 		// Right face
// 		 1.0f * width, -1.0f * height, -1.0f * shrek,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
// 		 1.0f * width, -1.0f * height,  1.0f * shrek,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
// 		 1.0f * width,  1.0f * height,  1.0f * shrek,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
// 		 1.0f * width,  1.0f * height, -1.0f * shrek,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//
// 		// Top face
// 		-1.0f * width,  1.0f * height, -1.0f * shrek,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
// 		 1.0f * width,  1.0f * height, -1.0f * shrek,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
// 		 1.0f * width,  1.0f * height,  1.0f * shrek,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
// 		-1.0f * width,  1.0f * height,  1.0f * shrek,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
//
// 		// Bottom face
// 		-1.0f * width, -1.0f * height, -1.0f * shrek,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
// 		 1.0f * width, -1.0f * height, -1.0f * shrek,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
// 		 1.0f * width, -1.0f * height,  1.0f * shrek,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
// 		-1.0f * width, -1.0f * height,  1.0f * shrek,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f
// 	};
//
// 	GLushort indices[] =
// 	{
// 		// Front
// 		0, 1, 2,  2, 3, 0,
// 		// Back
// 		4, 5, 6,  6, 7, 4,
// 		// Left
// 		8, 9,10, 10,11, 8,
// 		// Right
// 		12,13,14, 14,15,12,
// 		// Top
// 		16,17,18, 18,19,16,
// 		// Bottom
// 		20,21,22, 22,23,20
// 	};
//
// 	return Mesh(vertices, sizeof(vertices), indices, sizeof(indices));
// }
//
// Mesh Mesh::plane(float width, float height)
// {
// 	GLfloat vertices[] =
// 	{
// 		// position                           // normal            // texcoord
// 		-1.0f * width,  0.0f, -1.0f * height,  0.0f,  1.0f,  0.0f,  1.0f * width, 1.0f * height,
// 		 1.0f * width,  0.0f, -1.0f * height,  0.0f,  1.0f,  0.0f,  0.0f        , 1.0f * height,
// 		 1.0f * width,  0.0f,  1.0f * height,  0.0f,  1.0f,  0.0f,  0.0f        , 0.0f,
// 		-1.0f * width,  0.0f,  1.0f * height,  0.0f,  1.0f,  0.0f,  1.0f * width, 0.0f
// 	};
//
// 	GLushort indices[] =
// 	{
// 		0, 1, 2,  2, 3, 0
// 	};
//
// 	return Mesh(vertices, sizeof(vertices), indices, sizeof(indices));
// }
