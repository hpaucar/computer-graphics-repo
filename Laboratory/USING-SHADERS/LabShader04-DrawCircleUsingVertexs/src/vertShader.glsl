#version 410
uniform float offset;
const float radius = 1;
const float pi = 3.141592653589793;
const float twicePi = 2*pi;
const float numberOfSides = 100; //Is the same that number of vertex in main.cpp 49 line

void main(void) {
	//gl_VertexID: indica el id de los vertices en el buffer.   
    float i = gl_VertexID;
	float x_vertex = radius * cos(i * twicePi / numberOfSides);
	float y_vertex = radius * sin(i * twicePi / numberOfSides);
	
	gl_Position = vec4(x_vertex, y_vertex, 0.0, 1.0); 		    		
}
