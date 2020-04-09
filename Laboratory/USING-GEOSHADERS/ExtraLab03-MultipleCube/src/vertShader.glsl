#version 410

//in vec3 pos;
layout (location = 0) in vec3 pos;
in vec3 color;

out vec3 vColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(){
	//gl_Position = vec4(pos, 0.0, 1.0);
	gl_Position = proj_matrix * mv_matrix * vec4(pos, 1.0);  
	vColor = color;
}


