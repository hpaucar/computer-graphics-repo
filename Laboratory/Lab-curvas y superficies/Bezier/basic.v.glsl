#version 450

in vec2 coord2d;
in vec3 color;
out vec3 f_color;

void main(void){
    gl_Position = vec4(coord2d, 0, 1);
    f_color = color;
}
