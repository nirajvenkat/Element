#version 400 core

out vec4 color;

uniform vec2 iResolution;
uniform float time;

float map(vec3 p){
    float radius = 2.0 + 0.1*sin(25.0*p.x)*sin(p.y*25.0);
    return length(p) - radius;
}

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
    
    // Raymarch
    vec3 r0 = vec3(0.0, 0.0, 3.0);
    vec3 rd = vec3(p, -1.0);
    
    float tmax = 20.0;
    float h = 0.001;
    float t = 0.0;
    for (int i = 0; i < 100; i++){
        if(h < 0.0001 || t > tmax) break;
		h = map(r0 + t*rd);
        t += h;
    }
    
    if(t < tmax){
        vec3 pos = r0 + t*rd;
        vec2 e = vec2(0.0001, 0.0);
        vec3 normal = normalize(vec3(map(pos + e.xyy) - map(pos - e.xyy),
                                     map(pos + e.yxy) - map(pos - e.yxy),
                                     map(pos + e.yyx) - map(pos - e.yyx)));        
        col = vec3(1.0, 0.5, 0.2);
        vec3 movinglight = vec3(5.0*cos(time), 0.0, 5.0*sin(time));
        vec3 light = vec3(1.0);
        col *= clamp(dot(light,normal), 0.0, 1.0);
        vec3 col2 = vec3(0.0, 1.0, 0.0);
        col += col2 * clamp(dot(movinglight,normal), 0.0, 1.0);
        

    }
	else discard;
	
    /*
    //Raymarch 2
    vec3 r1 = vec3(5.0*sin(iGlobalTime), 5.0*cos(iGlobalTime), 7.0);

    h = 0.001;
    t = 0.0;
    for (int i = 0; i < 100; i++){
        if(h < 0.0001 || t > tmax) break;
		h = map(r1 + t*rd);
        t += h;
    }
    
    if(t < tmax){
        col *= pow(col, vec3(0.1));
    }
    */
    color = vec4(col, 1.);
}