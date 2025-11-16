#include "Model.hpp"

#include <filesystem>
#include "Material.hpp"

Model::Model(const char* path)
{
	std::filesystem::path modelPath(path);
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_PreTransformVertices);
	m_NumMeshes = scene->mNumMeshes;
	m_Meshes = new Mesh[m_NumMeshes];

	GLfloat* vertices;
	GLushort* indices;

	for (int i = 0; i < m_NumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		vertices = new GLfloat[mesh->mNumVertices * 8];
		indices = new GLushort[mesh->mNumFaces * 3];

		if (mesh->HasTextureCoords(0))
		{
			for (int j = 0; j < mesh->mNumVertices; j++)
			{
				aiVector3d vertex = mesh->mVertices[j];
				aiVector3d normal = mesh->mNormals[j];
				aiVector3d texCoords = mesh->mTextureCoords[0][j];
				vertices[(j*8)+0] = vertex.x;
				vertices[(j*8)+1] = vertex.y;
				vertices[(j*8)+2] = vertex.z;
				vertices[(j*8)+3] = normal.x;
				vertices[(j*8)+4] = normal.y;
				vertices[(j*8)+5] = normal.z;
				vertices[(j*8)+6] = texCoords.x;
				vertices[(j*8)+7] = texCoords.y;
			}
		}
		else
		{
			for (int j = 0; j < mesh->mNumVertices; j++)
			{
				aiVector3d vertex = mesh->mVertices[j];
				aiVector3d normal = mesh->mNormals[j];
				vertices[(j*8)+0] = vertex.x;
				vertices[(j*8)+1] = vertex.y;
				vertices[(j*8)+2] = vertex.z;
				vertices[(j*8)+3] = normal.x;
				vertices[(j*8)+4] = normal.y;
				vertices[(j*8)+5] = normal.z;
				vertices[(j*8)+6] = 0;
				vertices[(j*8)+7] = 0;
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString diffuseFile;
		aiString roughnessFile;

		material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseFile);
		material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &roughnessFile);

		std::filesystem::path diffusePath = modelPath.parent_path() / diffuseFile.C_Str();
		std::string diffuseString = diffusePath.string();

		std::filesystem::path roughnessPath = modelPath.parent_path() / roughnessFile.C_Str();
		std::string roughnessString = roughnessPath.string();

		Material mat(material->GetName().C_Str(), diffuseString.c_str(), roughnessString.c_str());

		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			indices[(j*3)+0] = face.mIndices[0];
			indices[(j*3)+1] = face.mIndices[1];
			indices[(j*3)+2] = face.mIndices[2];
		}

		m_Meshes[i].init(vertices, sizeof(GLfloat) * mesh->mNumVertices * 8, indices, sizeof(GLushort) * mesh->mNumFaces * 3, mat);
		delete[] vertices;
	}
}

void Model::draw()
{
	for (int i = 0; i < m_NumMeshes; i++)
		m_Meshes[i].draw();
}
