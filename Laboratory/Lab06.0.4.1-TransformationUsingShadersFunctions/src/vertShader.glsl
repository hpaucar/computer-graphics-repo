#version 410
layout (location=0) in vec3 position;  // coord
uniform float offset;

// funtions to Transformations(Rotate, Translate, Scale), check each matrix is a Transposed
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float dx, float dy, float dz);
mat4 buildScale(float sx, float sy, float sz);
const float pi = 3.141592653589793;

void main(void) {
	//All Transformation funtions. 
	mat4 localTrans = buildTranslate(offset, 0, 0); //Translate only in x axis.
	mat4 localScal = buildScale(0.25, 0.25, 0); //Scale in 25%.
	mat4 localRotaX = buildRotateX(offset*pi/4);
	mat4 localRotaY = buildRotateY(offset*pi/4);
	mat4 localRotaZ = buildRotateZ(offset*pi/4);
	
	//gl_Position = localTrans * vec4(position, 1.0);  // right-to-left
	//gl_Position = localScal * vec4(position, 1.0); 
	//gl_Position = localRotaZ * vec4(position, 1.0);	    		
	//gl_Position =  localTrans * localScal * vec4(position, 1.0);
	//gl_Position = localScal * localTrans * vec4(position, 1.0); // Why is more slow?
	gl_Position = localTrans * localRotaZ * localScal * vec4(position, 1.0);
}

// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(1.0, 0.0,      0.0,       0.0,
                     0.0, cos(rad), -sin(rad), 0.0,
                     0.0, sin(rad), cos(rad),  0.0,
                     0.0, 0.0,      0.0,       1.0);
    return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad) {
    mat4 yrot = mat4(cos(rad),  0.0, sin(rad), 0.0,
                     0.0,       1.0, 0.0,      0.0,
                     -sin(rad), 0.0, cos(rad), 0.0,
                     0.0,       0.0, 0.0,      1.0);
    return yrot;
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
mat4 buildTranslate(float dx, float dy, float dz) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                       dx,  dy,  dz, 1.0);
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
