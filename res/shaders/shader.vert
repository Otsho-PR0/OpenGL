#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
};

out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vPos;
out vec3 vShadowCoords;
out mat3 vTBN;


uniform mat4 uCamera;
uniform mat4 uModel;
uniform float uTime;

uniform mat4 uShadowCoords;

void main()
{
	gl_Position = uCamera * uModel * vec4(aPos, 1.0f);

	vec3 T = normalize(mat3(transpose(inverse(uModel))) * aTangent);
	vec3 B = normalize(mat3(transpose(inverse(uModel))) * aBitangent);
	vec3 N = normalize(mat3(transpose(inverse(uModel))) * aNormal);
	vTBN = mat3(T, B, N);

	mat4 biasMatrix = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	vNormal             = mat3(transpose(inverse(uModel))) * aNormal;
	vTexCoords          = aTexCoords;
	vShadowCoords       = vec3(biasMatrix * uShadowCoords * uModel * vec4(aPos, 1.0f));

	vPos                = vec3(uModel * vec4(aPos, 1.0f));
}
