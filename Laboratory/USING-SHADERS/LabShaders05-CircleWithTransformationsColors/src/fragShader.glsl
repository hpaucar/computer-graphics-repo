#version 410

in vec3 oColor;
out vec4 color;

void main(void){
	color = vec4(oColor, 1.0);	
}
