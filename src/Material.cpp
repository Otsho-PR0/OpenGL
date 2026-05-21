#include <Material.hpp>

#include <glad/gl.h>

void Material::Bind(Shader& shader)
{
	shader.SetVec3("uAlbedo", &albedo[0]);
	shader.SetFloat("uAO", ao);
	shader.SetFloat("uRoughness", roughness);
	shader.SetFloat("uMetalness", metalness);
	tDiffuse.Bind(GL_TEXTURE0);
	tAO.Bind(GL_TEXTURE1);
	tNormals.Bind(GL_TEXTURE2);
	tRoughness.Bind(GL_TEXTURE3);
	tMetalness.Bind(GL_TEXTURE4);
	tEmissive.Bind(GL_TEXTURE5);
}
