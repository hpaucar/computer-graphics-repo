#version 410
#ifdef GL_ES
precision mediump float;
#endif

uniform float u_time;
out vec4 fragColor;

// verify that point is in circle (x-a)^2+(x-b)^2 <= r^2
bool isInCircle(float x, float y){
	
	if(pow(x-400, 2)+pow(y-400, 2) <= 40000)
		return true;
	return false;	 
}

void drawCircle(){
	if(isInCircle(gl_FragCoord.x, gl_FragCoord.y))
		fragColor = vec4(abs(sin(u_time)), 0.0, 1.0, 1.0); 
	else
		fragColor = vec4(abs(sin(u_time)), 1.0, 1.0, 1.0); 		
}

void drawRectangle(){
	if(gl_FragCoord.x > 400.0f && gl_FragCoord.y > 400.0f)
		fragColor = vec4(abs(sin(u_time)), 0.0, 1.0, 1.0); 
	else if(gl_FragCoord.x < 400.0f && gl_FragCoord.y < 400.0f)
		fragColor = vec4(abs(sin(u_time)), 0.0, 1.0, 1.0); 
	else
		fragColor = vec4(abs(sin(u_time)), 1.0, 1.0, 1.0); 	
}

void main() {
	//drawRectangle();
	drawCircle();
}
