#version 400 core

out vec4 color;

//uniform sampler2D texture_diffuse1;
uniform vec2 iResolution;
uniform float time;

void main()
{   
	float col = 0.0;
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
    //vec3 texcol = texture(texture_diffuse1, uv).rgb;
	vec2 p = uv - 0.5;
	col = length(p) * (0.5+(0.5*sin(time)));
	if(col < 0.4) discard;
	color = vec4(col, 0.0, 0.0, 1.0);
}