#pragma once

#include "Mesh.hpp"
#include "Transform.hpp"

class Model
{
public:
	Model(const Model&) = delete;
	Model(const char*);
	~Model() = default;

	Mesh* operator[](unsigned int);

	void Draw(Shader&);

	Transform transform;

private:
	Mesh* m_Meshes;
	unsigned int m_NumMeshes;
};
