#version 330

out vec4 FragColor;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoords;

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	int type;
};

uniform sampler2D uMaterialDiffuse;
uniform sampler2D uMaterialRoughness;
uniform sampler2D uMaterialNormals;
uniform vec3 uCameraPosition;
uniform Light light;
uniform int doIt;

vec3 calcDirectionalLight()
{
	vec4 albedo = texture(uMaterialDiffuse, vTexCoords);
	vec4 roughness = texture(uMaterialRoughness, vTexCoords);
	vec3 normals = vec3(texture(uMaterialNormals, vTexCoords).rgb * 2 - 1);

	if (albedo.a < 0.25f)
		discard;

	vec3 ambient = light.ambient * albedo;

	vec3 normal = normalize(vNormal);
	float diff = max(dot(-light.direction, normal), 0);
	vec3 diffuse = diff * light.diffuse * albedo;

	vec3 viewDir = normalize(uCameraPosition - vPos);
	vec3 reflectDir = reflect(light.direction, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), light.shininess);
	vec3 specular = spec * light.specular * roughness.g;

	return ambient + diffuse + specular;
}

void main()
{
	vec4 color;
	color = vec4(calcDirectionalLight(), 1.0f);
	if (color.a < 0.25f)
		discard;
	FragColor = color;
}
