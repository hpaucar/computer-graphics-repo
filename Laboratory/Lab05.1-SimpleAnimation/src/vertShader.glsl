#version 410
uniform float offset;

void main(void) {
	//gl_VertexID: indica el id de los vertices en el buffer.
    if (gl_VertexID == 0)
    	gl_Position = vec4(0.25 + offset, -0.25, 0.0, 1.0);
    else if (gl_VertexID == 1)
    	gl_Position = vec4(-0.25 + offset, -0.25, 0.0, 1.0);
    else if (gl_VertexID == 2)
    	gl_Position = vec4(0.25 + offset, 0.25, 0.0, 1.0);

    //else if (gl_VertexID == 3)
    	//gl_Position = vec4(-0.25 + offset, 0.25, 0.0, 1.0);
    //else if (gl_VertexID == 4)
    	//gl_Position = vec4(0.25 + offset, 0.25, 0.0, 1.0);
    //else if (gl_VertexID == 5)
    	//gl_Position = vec4(-0.25 + offset, -0.25, 0.0, 1.0);    	    		
}
