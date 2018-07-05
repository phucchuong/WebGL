#include "Application.h"
#include "FileSystem.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Input.h"

Shaders myShaders;

GLuint vertexBuffer, color, indexBuffer, texturebuffer, texcoordBuffer;

// create the application here.
Application::Application()
{
}

char * textureData;
int width, height, bpp;
// Initialize current game.
int Application::Init(const char *resPath)
{
	FileSystem::GetInstance()->Init(resPath);
	//creation of shaders and program 
	myShaders.Init("Shaders/Basic.vs", "Shaders/Basic.fs");
	// load texture file

	textureData = LoadTGA("Train.tga", &width, &height, &bpp);
	if (!textureData) {
		return 0;
	}

	GLbyte indices[6] = { 0,3,1,1,2,0 };
	Vertex vertexData[4];

	vertexData[0].pos = Vector3(0.5, 0.5, 0);
	vertexData[1].pos = Vector3(-0.5, -0.5, 0);
	vertexData[2].pos = Vector3(0.5, -0.5, 0);
	vertexData[3].pos = Vector3(-0.5, 0.5, 0);

	GLfloat TexCoord[8] = { 1,1, 0,0, 1,0, 0,1 };

	//buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glGenTextures(1, &texturebuffer);
	glBindTexture(GL_TEXTURE_2D, texturebuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (bpp == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	}
	//delete[] textureData;
	glBindTexture(GL_TEXTURE_2D, NULL);

	glGenBuffers(1, &texcoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord), TexCoord, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	return true;
}

// Update current state
void Application::Update(float deltatime)
{
	Input * input = Input::GetInstance();

	if (input->HasPointer(POINTER_DRAGGED))
	{
		LOGI("\nPOINTER_DRAGGED");
	}
	if (input->HasPointer(POINTER_DOWN))
	{
		LOGI("\nPOINTER_DOWN");
	}
	if (input->HasPointer(POINTER_UP))
	{
		LOGI("\nPOINTER_UP");
	}

	if (input->KeyPressed())
	{
		LOGI("\nKey Pressed");
	}
}

// Render application

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glUseProgram(myShaders.program);

	if (myShaders.a_position != -1)
	{
		glEnableVertexAttribArray(myShaders.a_position);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(myShaders.a_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (myShaders.a_texcoord != -1) {
		glEnableVertexAttribArray(myShaders.a_texcoord);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
		glVertexAttribPointer(myShaders.a_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (myShaders.a_texture != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturebuffer);
		glUniform1i(myShaders.a_texture, 0);
	}

	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}

// destroy the application here.
void Application::Destroy()
{
	Application::FreeInstance();
};

Application::~Application()
{
	FileSystem::FreeInstance();
	Input::FreeInstance();
}

