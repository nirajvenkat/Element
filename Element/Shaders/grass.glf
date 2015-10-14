#version 400 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
    vec4 texColor = vec4(texture(texture_diffuse1, TexCoords));
	if(color.a < 0.1) discard;
    color = texColor;
}