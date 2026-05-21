#include <Mesh.hpp>
#include <Material.hpp>

#include <glad/gl.h>

Mesh::Mesh(Material* material, Vertex* vertices, size_t sVertices, unsigned int* indices, size_t sIndices) :
	count(sIndices / sizeof(unsigned int)),
	material(material)
{
	glGenVertexArrays(1, &m_ID);
	glBindVertexArray(m_ID);
	
	GLuint VBO, EBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(4);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sIndices, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Draw(Shader& shader)
{
	shader.SetMat4("uModel", &transform.GetModel()[0][0]);
	material->Bind(shader);
	glBindVertexArray(m_ID);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

Mesh* Mesh::cube()
{
	Vertex* vertices = nullptr;

	unsigned int* indices = nullptr;

	return new Mesh(new Material, vertices, sizeof(vertices), indices, sizeof(indices));
}

Mesh* Mesh::plane(float x, float y)
{
	Vertex vertices[] =
	{
		{ { -0.5f * x, 0.0f, -0.5f * y }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ {  0.5f * x, 0.0f, -0.5f * y }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ { -0.5f * x, 0.0f,  0.5f * y }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ {  0.5f * x, 0.0f,  0.5f * y }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } }
	};

	unsigned int indices[] =
	{
		0u, 2u, 1u,
		1u, 2u, 3u
	};

	return new Mesh(new Material, vertices, sizeof(vertices), indices, sizeof(indices));
}