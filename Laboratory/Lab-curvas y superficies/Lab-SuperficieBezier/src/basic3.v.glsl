#version 450

in vec3 coord3d;
in vec3 color;
uniform mat4 mvp;

out vec3 f_color;

void main(void){
    gl_Position = mvp * vec4(coord3d, 1);
    f_color = color;
}
