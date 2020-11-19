#version 410
#ifdef GL_ES
precision mediump float;
#endif

uniform float u_time;
out vec4 fragColor;

float temp;
void main() {
	fragColor = vec4(abs(sin(u_time)),0.0,0.0,1.0);
	//temp =1 - u_time/100;
	//fragColor = vec4(temp, temp, temp, 1.0);
	
}