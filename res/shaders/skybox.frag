#version 330

out vec4 FragColor;
in vec3 vTexCoords;

uniform sampler2D uTexture;
const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
	uv.y = 1.0f - uv.y;
    return uv;
}

uniform float uSkyboxIntensity;

void main()
{
    vec3 color = texture(uTexture, SampleSphericalMap(normalize(vTexCoords))).rgb * uSkyboxIntensity;
	FragColor = vec4(color / (color + vec3(1.0f)), 1.0f);
}
