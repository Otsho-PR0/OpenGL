#pragma once

#include "Shader.hpp"

class Skybox
{
public:
	Skybox() = delete;
	Skybox(const char*);

	void Draw(float* camera);
	void SetSkyboxIntensity(float intensity)
	{
		m_Shader.SetFloat("uSkyboxIntensity", intensity);
	}

private:
	unsigned int m_ID;
	unsigned int m_Texture;
	Shader m_Shader;
};
