attribute vec3 a_position;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat4 mMatrix;

attribute vec2 texcoord;
varying vec2 vtexcoord;

void main(){
	gl_Position = vec4(a_position,1.0);
	gl_Position = pMatrix * vMatrix * mMatrix * gl_Position;
	vtexcoord = texcoord;
}