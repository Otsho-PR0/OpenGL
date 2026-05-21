#version 330

layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 uCamera;

void main()
{
	gl_Position = uCamera * vec4(aPos, 1.0f);
	vTexCoords = aPos;
}
