#version 150

in vec2 pos;
in vec3 color;

out vec3 vColor;

void main(){
	gl_Position = vec4(pos, 0.0, 1.0);
	vColor = color;
}