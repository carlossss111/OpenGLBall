#version 410 core

layout (location = 0) in vec3 vPos;

uniform mat4 projectedLightSpace;
uniform mat4 model;

void main() {
	gl_Position = projectedLightSpace * model * vec4(vPos, 1.f);
}
