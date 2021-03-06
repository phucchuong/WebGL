#pragma once
#include "Engine.h"
class Shaders
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint a_position, a_color, a_texcoord, a_texture,pMatrix,vMatrix,mMatrix;
	GLuint LoadProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint LoadShader(GLenum type, const char * filename);
	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};