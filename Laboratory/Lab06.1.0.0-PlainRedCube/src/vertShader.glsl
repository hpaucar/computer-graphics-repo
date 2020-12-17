#version 410

layout (location = 0) in vec3 position;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

void main(void) {
	//M->V->P
    gl_Position = proj_matrix * v_matrix * m_matrix * vec4(position, 1.0);
}
