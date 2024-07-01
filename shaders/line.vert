#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;

out vec3 fPos;
out vec3 fCol;

uniform mat4 view;
uniform mat4 projection;

void main() {
    fCol = vCol;
	gl_Position = projection * view * vec4(vPos,1.f);
}
