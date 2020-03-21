#version 450
out vec4 fragColor;
vec2 res;
void main(void)
{ 
	res = vec2(800.0, 800.0);
    vec2 p = gl_FragCoord.xy/res.xy;    
    vec3 col = vec3(1.0, 1.2, 0.1);
   
   // Draw vertical
   // col += p.x;    
   
   // Draw circle
    col *= length(p - vec2(0.5, 0.5));
   
    fragColor = vec4(col, 1.0);
}