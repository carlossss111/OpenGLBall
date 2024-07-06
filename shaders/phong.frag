#version 410 core

in vec3 fPos;
in vec3 fNor;
in vec2 fTex;
in vec4 fLightSpace;

out vec4 fColour;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	bool hasSpecular;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;
uniform sampler2D depthMap;

float calculateShadow() {
	vec3 projCoords = fLightSpace.xyz / fLightSpace.w;
	projCoords = projCoords * 0.5f + 0.5f;
	float closestDepth = texture(depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	vec3 normal = normalize(fNor);
	vec3 lightDir = normalize(light.position - fPos);
	float bias = max(0.05f * (1.f - dot(normal, lightDir)), 0.005f);

	float shadow = 0.0f;
	vec2 texelSize = 1.f / textureSize(depthMap, 0);
	for (int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(depthMap, projCoords.xy + vec2(x,y) * texelSize).r;
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
	vec3 lightDir = normalize(light.position - fPos);
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
	float diffTransparency = texture(material.diffuse, fTex).a;
    if(diffTransparency < 0.1f){
		discard;
	}
	fColour = vec4(calculateDirectionalLight(), 1.f);
}
