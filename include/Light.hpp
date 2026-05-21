#pragma once

#include <vec3.hpp>
#include "Shader.hpp"
#include "Transform.hpp"

class Light
{
public:
	enum class Type : unsigned char
	{
		Directional,
		Point,
		Spot
	};

public:
	Light(const Light&) = delete;
	Light(Type);

	void Setup(Shader&);
	void BindShadowMap();
	void UnBind();

	unsigned int GetShadowMap();

	Type type;

	Transform transform;

	float intensity;

private:
	unsigned int m_ID;
	unsigned int m_FrameBuffer;
	unsigned int m_ShadowMap;
};
