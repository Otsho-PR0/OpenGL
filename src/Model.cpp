#include <Model.hpp>

#include <glad/gl.h>

#include <iostream>
#include <vector>
#include <filesystem>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void ProcessNodes(Mesh* m_Meshes, aiNode* node)
{
	static aiVector3D parentScale(1.0f);
	static aiVector3D parentRotation;
	static aiVector3D parentPosition;

	aiVector3D scale;
	aiVector3D rotation;
	aiVector3D position;
	node->mTransformation.Decompose(scale, rotation, position);
	scale.x = scale.x * parentScale.x;
	scale.y = scale.y * parentScale.y;
	scale.z = scale.z * parentScale.z;
	rotation += parentRotation;
	position += parentPosition;

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		m_Meshes[node->mMeshes[i]].transform.scale.x = scale.x;
		m_Meshes[node->mMeshes[i]].transform.scale.y = scale.y;
		m_Meshes[node->mMeshes[i]].transform.scale.z = scale.z;

		m_Meshes[node->mMeshes[i]].transform.rotation.x = rotation.x;
		m_Meshes[node->mMeshes[i]].transform.rotation.y = rotation.y;
		m_Meshes[node->mMeshes[i]].transform.rotation.z = rotation.z;

		m_Meshes[node->mMeshes[i]].transform.position.x = position.x;
		m_Meshes[node->mMeshes[i]].transform.position.y = position.y;
		m_Meshes[node->mMeshes[i]].transform.position.z = position.z;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		static aiVector3D pScale = parentScale;
		static aiVector3D pRotation = parentRotation;
		static aiVector3D pPosition = parentPosition;

		parentScale = scale;
		parentRotation = rotation;
		parentPosition = position;

		ProcessNodes(m_Meshes, node->mChildren[i]);

		parentScale = pScale;
		parentRotation = pRotation;
		parentPosition = pPosition;
	}
}

Model::Model(const char* path)
{
	std::filesystem::path dir(path);
	if (!std::filesystem::exists(dir))
	{
		std::cerr << "Failed to open file: " << path << std::endl;
		m_NumMeshes = 0;
		m_Meshes = nullptr;
		return;
	}
	dir = dir.parent_path();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace
		);

	if (!scene)
	{
		std::cerr << "Failed to load model: " << importer.GetErrorString() << std::endl;
		m_NumMeshes = 0;
		m_Meshes = nullptr;
		return;
	}

	if (scene->mNumMeshes == 0)
	{
		std::cerr << "Model has no meshes" << std::endl;
		m_NumMeshes = 0;
		m_Meshes = nullptr;
		return;
	}

	m_NumMeshes = scene->mNumMeshes;
	m_Meshes = new Mesh[m_NumMeshes];

	for (unsigned int i = 0; i < m_NumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
			vertices.push_back(
				{
					{mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z},
					{mesh->HasNormals() ? mesh->mNormals[j].x : 0.0f, mesh->HasNormals() ? mesh->mNormals[j].y : 0.0f, mesh->HasNormals() ? mesh->mNormals[j].z : 0.0f},
					{mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j].x : 0.0f, mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j].y : 0.0f},
					{mesh->HasTangentsAndBitangents() ? mesh->mTangents[j].x : 0.0f, mesh->HasTangentsAndBitangents() ? mesh->mTangents[j].y : 0.0f, mesh->HasTangentsAndBitangents() ? mesh->mTangents[j].z : 0.0f},
					{mesh->HasTangentsAndBitangents() ? mesh->mBitangents[j].x : 0.0f, mesh->HasTangentsAndBitangents() ? mesh->mBitangents[j].y : 0.0f, mesh->HasTangentsAndBitangents() ? mesh->mBitangents[j].z : 0.0f}
				}
			);

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			if (mesh->mFaces[j].mNumIndices == 3)
			{
				indices.push_back(mesh->mFaces[j].mIndices[0]);
				indices.push_back(mesh->mFaces[j].mIndices[1]);
				indices.push_back(mesh->mFaces[j].mIndices[2]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiString dPath;
		aiString aPath;
		aiString nPath;
		aiString rPath;
		aiString mPath;
		aiString ePath;
		aiColor3D albedo;
		float ao = 1.0f;
		float roughness;
		float metalness;

		material->Get(AI_MATKEY_BASE_COLOR, albedo);
		material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
		material->Get(AI_MATKEY_METALLIC_FACTOR, metalness);
		material->GetTexture(aiTextureType_DIFFUSE, 0, &dPath);
		material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &aPath);
		material->GetTexture(aiTextureType_NORMALS, 0, &nPath);
		material->GetTexture(aiTextureType_GLTF_METALLIC_ROUGHNESS, 0, &rPath);
		material->GetTexture(aiTextureType_METALNESS, 0, &mPath);
		material->GetTexture(aiTextureType_EMISSIVE, 0, &ePath);

		Material* mat = new Material();
		mat->albedo.r = albedo.r;
		mat->albedo.g = albedo.g;
		mat->albedo.b = albedo.b;
		mat->ao = ao;
		mat->roughness = roughness;
		mat->metalness = metalness;
		if (!dPath.Empty())
			mat->tDiffuse = Texture((dir / dPath.C_Str()).string().c_str());
		if (!aPath.Empty())
			mat->tAO.GiveID(Texture::LoadTexture((dir / aPath.C_Str()).string().c_str(), GL_TEXTURE_2D, GL_RGB16, GL_RGB));
		if (!nPath.Empty())
			mat->tNormals.GiveID(Texture::LoadTexture((dir / nPath.C_Str()).string().c_str(), GL_TEXTURE_2D, GL_RGB16, GL_RGB));
		if (!rPath.Empty())
			mat->tRoughness.GiveID(Texture::LoadTexture((dir / rPath.C_Str()).string().c_str(), GL_TEXTURE_2D, GL_RGB16, GL_RGB));
		if (!mPath.Empty())
			mat->tMetalness.GiveID(Texture::LoadTexture((dir / mPath.C_Str()).string().c_str(), GL_TEXTURE_2D, GL_RGB16, GL_RGB));
		if (!ePath.Empty())
			mat->tEmissive.GiveID(Texture::LoadTexture((dir / ePath.C_Str()).string().c_str(), GL_TEXTURE_2D, GL_RGB16, GL_RGB));

		m_Meshes[i] = Mesh(mat, vertices.data(), vertices.size() * sizeof(Vertex), indices.data(), indices.size() * sizeof(unsigned int));
	}

	ProcessNodes(m_Meshes, scene->mRootNode);
}

Mesh* Model::operator[](unsigned int i)
{
	return &m_Meshes[i];
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < m_NumMeshes; i++)
		m_Meshes[i].Draw(shader);
}
