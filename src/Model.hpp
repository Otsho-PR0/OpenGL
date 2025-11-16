#pragma once

#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model(const char* path);

	void draw();
private:
	Mesh* m_Meshes;
	int m_NumMeshes;
};
