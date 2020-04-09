#version 150

in vec3 finalColour;

out vec4 out_Colour;

void main(void){

	out_Colour = vec4(finalColour, 1.0);

}