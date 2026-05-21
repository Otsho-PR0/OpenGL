#include <Light.hpp>

#include <string>

#include <glad/gl.h>

Light::Light(Type type) :
	intensity(1.0f),
	type(type),
	m_ID(0u)
{
	glGenTextures(1, &m_ShadowMap);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
             1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat color = 0.0f;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &m_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::Setup(Shader& shader)
{
	shader.SetVec3(std::string("light[").append(std::to_string(m_ID)).append("].position").c_str(), &transform.position[0]);
	shader.SetVec3(std::string("light[").append(std::to_string(m_ID)).append("].direction").c_str(), &(transform.GetBasis() * glm::vec3(0.0f, 0.0f, -1.0f))[0]);
	shader.SetFloat(std::string("light[").append(std::to_string(m_ID)).append("].intensity").c_str(), intensity);
}

void Light::BindShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
}

void Light::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

unsigned int Light::GetShadowMap()
{
	return m_ShadowMap;
}