
attribute vec3 a_position;
attribute vec3 color;
varying vec3 vcolor;
void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = positionL;
	vcolor = color;
}
   