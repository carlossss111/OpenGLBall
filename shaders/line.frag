#version 410 core

in vec3 fCol;

out vec4 fColour;

void main() {
	fColour = vec4(fCol, 1.f);
}