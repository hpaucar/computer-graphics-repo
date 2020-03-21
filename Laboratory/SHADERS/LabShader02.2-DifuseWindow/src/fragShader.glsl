#version 450
#ifdef GL_ES
precision mediump float;
#endif

out vec4 fragColor;

//dimension of windows
uniform vec2 u_resolution;

void main(void) {
	vec2 st = gl_FragCoord.xy / u_resolution;
	fragColor = vec4(st.x,st.y,0.0,1.0);
}