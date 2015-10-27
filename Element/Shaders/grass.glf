#version 400 core

in vec2 TexCoords;

out vec4 color;

//uniform sampler2D texture1;

void main() {    
    //vec4 texColor = texture(texture1, TexCoords);
	//if(texColor.a < 0.1) discard;
    //color = texColor;
	
	vec3 black = vec3(0.0, 0.0, 0.0);
	vec3 green = vec3(0.0, 1.0, 0.0);
	//vec3 fcolor = mix(black, green, TexCoords.y);
	color = vec4(green, 1.0);
}