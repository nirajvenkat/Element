#version 400 core

out vec4 color;

uniform vec2 iResolution;

float sphere(vec3 point, float radius){
    return length(point) - radius;
}

float cube(float x,float y,float z,float size){
	return max(max(abs(x)-size,abs(y)-size),abs(z)-size);
}

float cylinder(vec3 p, float r, float c) {
    return max(length(p.xz)-r, abs(p.y)-c);
}

void main(){
    vec3 col = vec3(0.0);
    
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec2 p = uv*2.0 - 1.0;
    p.x *= iResolution.x/iResolution.y;
    
    vec3 r0 = vec3(0.0, 0.0, 0.2);
    vec3 rd = vec3(p, -1.0);
    
    float tmax = 20.0;
    float h = 0.001;
    float t = 0.0;
    for (int i = 0; i < 100; i++){
        if(h < 0.0001 || t > tmax) break;
		h = abs(sphere(t*rd - r0, 1.0));
        t += h;
    }
    
    if(t < tmax){
        col = vec3(1.0, 0.5, 0.2);
    }
	else discard;
    
    color = vec4(col, 1.0);
}