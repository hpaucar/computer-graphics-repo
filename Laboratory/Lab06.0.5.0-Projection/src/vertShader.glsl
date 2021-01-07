#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;

void main(){
	//Projection Matrix
	gl_Position = projection * vec4(aPos, 1.0);
}