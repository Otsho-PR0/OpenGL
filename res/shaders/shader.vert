#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	vPos = aPos;
	vTexCoords = aTexCoords;
	vNormal = mat3(transpose(inverse(uModel))) * aNormal;
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
}
