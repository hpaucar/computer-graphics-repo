#version 410

layout (location = 0) in vec3 position;

layout (location = 1) in vec3 vColor;
out vec4 vertexColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void) {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);  
    vertexColor = vec4(vColor,1.0); 
}
