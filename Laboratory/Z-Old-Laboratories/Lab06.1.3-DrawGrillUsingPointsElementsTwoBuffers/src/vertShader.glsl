#version 410

in vec3 iPosition;
in vec3 iColor;
out vec3 oColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void) {
	oColor = iColor;
    gl_Position = proj_matrix * mv_matrix * vec4(iPosition, 1.0);  
}