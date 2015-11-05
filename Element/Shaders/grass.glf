#version 400 core

//in float distFactorGS;

out vec4 color;

void main() {    
    //vec4 texColor = texture(texture1, TexCoords);
	//if(texColor.a < 0.1) discard;
    //color = texColor;
	
	vec3 black = vec3(0.0, 0.0, 0.0);
	vec3 green = vec3(0.0, 0.7, 0.0);
	//vec3 fcolor = mix(black, green, distFactorGS);
	color = vec4(green, 1.0);
}