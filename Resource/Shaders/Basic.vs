attribute vec3 a_position;
attribute vec2 texcoord;

varying vec2 vtexcoord;

void main()
{
	vec4 positionL = vec4(a_position, 1.0);
	gl_Position = positionL;
	vtexcoord = texcoord;
}
   