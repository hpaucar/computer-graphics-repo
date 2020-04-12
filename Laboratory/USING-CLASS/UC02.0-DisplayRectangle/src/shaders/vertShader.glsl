#version 330
// asignamos una posicion (1) a la variable 
layout (location = 1) in vec3 position;

out vec3 colour;

void main(void){
	gl_Position = vec4(position, 1.0);
	colour = vec3(position.x + 0.5, 0.0, position.y + 0.5);
}