
attribute vec3 a_position;
attribute vec3 color;
varying vec3 vcolor;
uniform mat4 scale;
void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = scale * positionL;
	vcolor = color;
}
   