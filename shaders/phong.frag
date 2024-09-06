#version 410 core

in vec3 fPos;
in vec3 fNor;
in vec2 fTex;

out vec4 fColour;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	bool hasSpecular;
};

struct Light {
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;
uniform sampler2DArray depthMap;

uniform mat4 view;
uniform float farPlane;
layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;

float calculateShadow() {
	// Cascade Layer Logic
	vec4 fragPosViewSpace = view * vec4(fPos, 1.f);
	float depthValue = abs(fragPosViewSpace.z);

	int layer = -1;
	for (int i = 0; i < cascadeCount; ++i){
		if (depthValue < cascadePlaneDistances[i]){
			layer = i;
			break;
		}
	}
	if (layer == -1){
		layer = cascadeCount;
	}

	vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fPos, 1.f);

	// Shadow Calculations
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5f + 0.5f;
	float currentDepth = projCoords.z;

	vec3 normal = normalize(fNor);
	vec3 lightDir = -normalize(light.direction);
	float bias = max(0.05f * (1.f - dot(normal, lightDir)), 0.005f);
	if(layer == cascadeCount){
		bias *= 1 / (farPlane * 0.5f);
	}
	else{
		bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);
	}

	float shadow = 0.0f;
	vec2 texelSize = 1.f / vec2(textureSize(depthMap, 0));
	for (int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(depthMap, vec3(projCoords.xy + vec2(x,y) * texelSize, layer)).r;
			shadow += currentDepth - bias > pcfDepth ? 1.f : 0.f;
		}
	}
	shadow /= 9.f;

	if(projCoords.z > 1.f){
		shadow = 0.f;
	}

	return shadow;
}

vec3 calculateDirectionalLight() {
	// Ambient
	vec3 ambient = light.ambient * texture(material.diffuse, fTex).rgb;

	// Diffuse
	vec3 norm = normalize(fNor);
	vec3 lightDir = -normalize(light.direction);
	float diff = max(dot(norm, lightDir), 0.f);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, fTex).rgb;

	// Specular
	vec3 specular = vec3(0.f,0.f,0.f);
	if(material.hasSpecular){
		vec3 viewDir = normalize(cameraPos - fPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
		specular = light.specular * spec * texture(material.specular, fTex).rgb;
	}

	// Shadow
	float shadow = calculateShadow();

	return ambient + (1.f - shadow) * (diffuse + specular);
}

void main() {
	float transparency = texture(material.diffuse, fTex).a;
    if(transparency < 0.1f){
		discard;
	}
	fColour = vec4(calculateDirectionalLight(), transparency);
}
