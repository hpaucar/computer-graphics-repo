#version 450
out vec4 fragColor;
vec2 resolution;
void main(void)
{ 
	resolution = vec2(800.0, 800.0);
    vec2 p = gl_FragCoord.xy/resolution.xy;    
    vec3 col = vec3(0, 0.8, 0.1);
   
   // Draw vertical
   // col += p.x;    
   
   // Draw circle
    col *= length(p - vec2(0.5, 0.5));
   
    fragColor = vec4(col, 1.0);
}