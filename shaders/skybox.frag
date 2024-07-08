#version 410 core

in vec3 fTex;

out vec4 fColour;

uniform samplerCube skybox;

void main() {    
    fColour = texture(skybox, fTex);
}