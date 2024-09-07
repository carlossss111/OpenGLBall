#version 410 core

in vec2 fTex;

out vec4 fColour;

uniform sampler2D depthMap;
uniform float nearPlane;
uniform float farPlane;

void main() {
	float depthValue = texture(depthMap, fTex).r;
	fColour = vec4(vec3(depthValue),1.f);
}
