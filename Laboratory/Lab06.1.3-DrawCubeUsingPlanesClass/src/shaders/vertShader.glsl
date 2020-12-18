#version 410

layout (location = 0) in vec3 position;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec4 oColor;
// funtions to Transformations(Rotate, Translate, Scale)
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float dx, float dy, float dz);
mat4 buildScale(float sx, float sy, float sz);
const float pi = 3.141592653589793;

void main(void) {
	float a, b, c = 0;
	mat4 localRot, localTrans, localModel; //Scale in 50%.
	oColor = vec4(0, 0, 0, 1);
	if(gl_InstanceID == 0){ //Triangle in I quadrant
		localRot = buildRotateX(0);
		localTrans = buildTranslate(0, 0, 1);
		oColor = vec4(1, 0, 0, 1);
	}else if(gl_InstanceID == 1){ //Triangle in I quadrant
		localRot = buildRotateX(pi/2);
		localTrans = buildTranslate(0, 1, 0);
		oColor = vec4(1, 1, 1, 1);
	}else if(gl_InstanceID == 2){ //Triangle in II quadrant
		localRot = buildRotateX(pi/2);
		localTrans = buildTranslate(0, -1, 0);
		oColor = vec4(0, 1, 0, 1);
	}else if(gl_InstanceID == 3){ //Triangle in III quadrant
		localRot = buildRotateY(pi/2);
		localTrans = buildTranslate(-1, 0, 0);
		oColor = vec4(0, 0, 1, 1);
	}else if(gl_InstanceID == 4){  //Triangle in IV quadrant
		localRot = buildRotateY(pi/2);
		localTrans = buildTranslate(1, 0, 0);
		oColor = vec4(0.1, 0.7, 0.5, 1);
	}else if(gl_InstanceID == 5){ //Triangle in III quadrant
		localRot = buildRotateX(0);
		localTrans = buildTranslate(0, 0, -1);
		oColor = vec4(1, 0.5, 0.3, 1);
	}
	
	//All Transformation funtions. 	
	localModel = buildTranslate(0, 0, -1); //Translate Origem 0, 0, 0.
	localModel = localTrans * localRot * localModel; 
	
	//M->V->P
    //gl_Position = proj_matrix * v_matrix * m_matrix * vec4(position, 1.0);
	gl_Position = proj_matrix * v_matrix * m_matrix * localModel *  vec4(position, 1.0);  // right-to-left
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
