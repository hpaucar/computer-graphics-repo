#version 410

uniform float rotate;
const float radius = 1;
const float pi = 3.141592653589793;
const float twicePi = 2*pi;
const float numberOfSides = 50;
out vec3 oColor;

mat4 buildRotateZ(float rad);
mat4 buildScale(float sx, float sy, float sz);

void main(void) {
	//gl_VertexID: indica el id de los vertices en el buffer.     
    if(gl_VertexID%2 == 0)
    	oColor = vec3(1.0, 0.0, 0.0);
    else
    	oColor = vec3(0.0, 1.0, 0.0);
    
    float i = gl_VertexID;    
	float x_vertex = radius * cos(i * twicePi / numberOfSides);
	float y_vertex = radius * sin(i * twicePi / numberOfSides);
	
	mat4 localRotaZ = buildRotateZ(rotate * twicePi / 360);
	mat4 localScal = buildScale(0.8, 0.8, 0); //Scale in 25%.

	//gl_Position = localRotaZ * vec4(x_vertex, y_vertex, 0.0, 1.0);
	gl_Position = localScal * localRotaZ * vec4(x_vertex, y_vertex, 0.0, 1.0); 		    		
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad), cos(rad),  0.0, 0.0,
                     0.0,      0.0,       1.0, 0.0,
                     0.0,      0.0,       0.0, 1.0);
    return zrot;
}

// builds and returns a Scale matrix
mat4 buildScale(float sx, float sy, float sz) {
    mat4 scale = mat4( sx, 0.0, 0.0, 0.0,
                      0.0,  sy, 0.0, 0.0,
                      0.0, 0.0,  sz, 0.0,
                      0.0, 0.0, 0.0, 1.0);
    return scale;
}