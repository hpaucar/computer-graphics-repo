#version 410
layout (location=0) in vec3 position;  // coord
uniform float rotate;

out vec3 oColor;

// funtions to Transformations(Rotate, Translate, Scale)
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 buildScale(float sx, float sy, float sz);

// Global Variables
const float radius = 0.2;
const float pi = 3.141592653589793;
const float twicePi = 2*pi;
const float numberOfSides = 12; //Numero de punto donde ser trasladan los triangulos

void main(void){	
	//gl_InstanceID: indica el id de los vertices en el buffer.     
    if(gl_InstanceID%2 == 0)
    	oColor = vec3(1.0, 0.0, 0.0);
    else
    	oColor = vec3(0.0, 1.0, 0.0);
    	
	float i = gl_InstanceID;
	float x_vertex = 0.5 + radius * cos(i * twicePi / numberOfSides);
	float y_vertex = 0.5 + radius * sin(i * twicePi / numberOfSides);
		
	mat4 localRotaZ = buildRotateZ(rotate * twicePi*4 / 360);
	
	//All Transformation funtions. 	
	mat4 localTrans = buildTranslate(x_vertex, y_vertex, 0); //Translate in x and y axis.
	mat4 localScal = buildScale(0.10, 0.06, 0); //Scale in 10% y 6%.

	gl_Position = localTrans * localRotaZ * localScal * vec4(position, 1.0);  // right-to-left
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad), cos(rad),  0.0, 0.0,
                     0.0,      0.0,       1.0, 0.0,
                     0.0,      0.0,       0.0, 1.0);
    return zrot;
}

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      x,   y,   z,   1.0);
    return trans;
}

// builds and returns a Scale matrix
mat4 buildScale(float sx, float sy, float sz) {
    mat4 scale = mat4( sx, 0.0, 0.0, 0.0,
                      0.0,  sy, 0.0, 0.0,
                      0.0, 0.0,  sz, 0.0,
                      0.0, 0.0, 0.0, 1.0);
    return scale;
}
