#version 400 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform vec2 iResolution;

void main()
{    
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec3 texcol = texture(texture_diffuse1, uv).rgb;
	vec2 p = uv - 0.5;

}