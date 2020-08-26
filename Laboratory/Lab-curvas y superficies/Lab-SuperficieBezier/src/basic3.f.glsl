#version 450

in vec3 f_color;
out vec4 FragColor;

void main(void){
    FragColor = vec4(f_color, 1.0);
}

