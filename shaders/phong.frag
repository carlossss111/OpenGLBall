#version 450 core

layout (location = 0) in vec3 fPos;
layout (location = 1) in vec3 fNor;
layout (location = 2) in vec2 fTex;

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

vec3 calculateDirectionalLight(){
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
	return ambient + diffuse + specular;
}

void main() {
	float diffTransparency = texture(material.diffuse, fTex).a;
    if(diffTransparency < 0.1f){
		discard;
	}
	fColour = vec4(calculateDirectionalLight(), 1.f);
}
