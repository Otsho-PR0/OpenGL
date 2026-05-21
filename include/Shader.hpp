#pragma once

class Shader
{
public:
	Shader() = delete;
	Shader(const Shader&) = delete;
	Shader(const char*, const char*);

	void Bind();
	void SetInt(const char*, int);
	void SetFloat(const char*, float);
	void SetVec3(const char*, float*);
	void SetMat4(const char*, float*);

private:
	unsigned int m_ID;
};
