#version 150
in vec3 oColor;
out vec4 outColor;
void main()
{
   outColor = vec4(oColor, 1.0);
}