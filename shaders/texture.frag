#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    vec4 fTex = texture(texture_diffuse1, TexCoords);
    if(fTex.a < 0.1){
		discard;
	}
    FragColor = fTex;
}