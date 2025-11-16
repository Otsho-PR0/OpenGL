#include "Material.hpp"

Material::Material()
{}

Material::Material(const char* name, const char* diffPath, const char* roughPath) :
	name(name),
	diff(diffPath),
	rough(roughPath)
{
}

void Material::init(const char* name, const char* diffPath, const char* roughPath)
{
	name = name;
	diff.init(diffPath);
	rough.init(roughPath);
}

void Material::use()
{
	diff.bind();
	rough.bind(GL_TEXTURE1);
}
