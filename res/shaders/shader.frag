#version 330

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
};

in vec3 vNormal;
in vec2 vTexCoords;
in vec3 vPos;
in vec3 vShadowCoords;
in mat3 vTBN;

uniform Light light[2];

out vec4 FragColor;

uniform sampler2D uTextureDiffuse;
uniform sampler2D uTextureAO;
uniform sampler2D uTextureNormals;
uniform sampler2D uTextureRoughness;
uniform sampler2D uTextureMetalness;
uniform sampler2D uTextureEmissive;

uniform sampler2D uTextureSkybox;
uniform float uIntensity;
uniform int uMode;
uniform sampler2D uShadowMap;

uniform vec3 uCameraPosition;

uniform bool uUseNormal;

uniform vec3 uAlbedo;
uniform float uAO;
uniform float uRoughness;
uniform float uMetalness;

const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{
	 vec3 albedo = length(texture(uTextureDiffuse, vTexCoords).rgb) > 0.0
	 	? texture(uTextureDiffuse, vTexCoords).rgb
	 	: uAlbedo;

	 float ao = length(texture(uTextureAO, vTexCoords).rgb) > 0.0
	 	? texture(uTextureAO, vTexCoords).r
	 	: uAO;

	 float roughness = length(texture(uTextureRoughness, vTexCoords).rgb) > 0.0f
	 	? texture(uTextureRoughness, vTexCoords).g
	 	: uRoughness;

	 float metalness = length(texture(uTextureMetalness, vTexCoords).rgb) > 0.0f
	 	? texture(uTextureMetalness, vTexCoords).b
	 	: uMetalness;


    vec3 V = normalize(uCameraPosition - vPos);
	vec3 N = vTBN * normalize(texture(uTextureNormals, vTexCoords).rgb * 2.0f - 1.0f);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metalness);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; ++i) 
    {
        vec3 L = normalize(-light[0].direction);
        vec3 H = normalize(V + L);
        
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metalness;	  
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * NdotL; 
    }   
  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));

	color += pow(texture(uTextureEmissive, vTexCoords).rgb, vec3(2.2f));

	switch (uMode)
	{
	case 0:
		FragColor = vec4(color, 1.0);
		break;
	case 1:
		FragColor = vec4(albedo, 1.0);
		break;
	case 2:
		FragColor = vec4(N * .5f + .5f, 1.0);
		break;
	case 3:
		FragColor = vec4(vNormal, 1.0);
		break;
	case 4:
		FragColor = vec4(vec3(roughness), 1.0);
		break;
	case 5:
		FragColor = vec4(vec3(metalness), 1.0);
		break;
	case 6:
		FragColor = vec4(vec3(vShadowCoords.z), 1.0);
		break;
	case 7:
		FragColor = vec4(vec3(texture(uShadowMap, vShadowCoords.xy).z), 1.0);
		break;
	}
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  