#version 410 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
layout (location = 2) in vec2 vTex;

out vec3 fPos;
out vec3 fNor;
out vec2 fTex;
out vec4 fLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 projectedLightSpace;

void main() {
	fPos = vec3(model * vec4(vPos, 1.f));
	fNor = mat3(transpose(inverse(model))) * vNor;
	fTex = vTex;
	fLightSpace = projectedLightSpace * vec4(fPos, 1.f);

	gl_Position = projection * view * vec4(fPos,1.f);
}
