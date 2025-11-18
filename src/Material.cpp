#include "Material.hpp"

Material::Material()
{}

Material::Material(const char* name, const char* diffPath, const char* roughPath, const char* normPath) :
	name(name),
	diff(diffPath),
	rough(roughPath),
	norm(normPath)
{
}

void Material::init(const char* name, const char* diffPath, const char* roughPath, const char* normPath)
{
	name = name;
	diff.init(diffPath);
	rough.init(roughPath);
	norm.init(normPath);
}

void Material::use()
{
	diff.bind();
	rough.bind(GL_TEXTURE1);
	norm.bind(GL_TEXTURE2);
}
