#version 410
layout (location=0) in vec3 position;  // coord

// funtions to Transformations(Rotate, Translate, Scale)
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 buildScale(float sx, float sy, float sz);
const float pi = 3.141592653589793;

void main(void) {
	float a, b, c = 0;
	if(gl_InstanceID == 0){ //Triangle in I quadrant
		a = b = 0.5;
	}else if(gl_InstanceID == 1){ //Triangle in II quadrant
		a = -0.5; b = 0.5;
	}else if(gl_InstanceID == 2){ //Triangle in III quadrant
		a = -0.5; b = -0.5;
	}else if(gl_InstanceID == 3){  //Triangle in IV quadrant
		a = 0.5; b = -0.5;
	}
	
	//All Transformation funtions. 	
	mat4 localTrans = buildTranslate(a, b, 0); //Translate in x and y axis.
	mat4 localScal = buildScale(0.5, 0.5, 0); //Scale in 50%.
	
	gl_Position = localTrans * localScal * vec4(position, 1.0);  // right-to-left
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
