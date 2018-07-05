#include "Application.h"
#include "FileSystem.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Input.h"

# define M_PI           3.14159265358979323846
Shaders myShaders;
/// triangle
Vertex vertexData[3];

float * proj_matrix, *mov_matrix, *view_matrix;

float* get_projection(float angle, float a, float zMin, float zMax) {
	float ang = (float)tanf((angle*0.5)*M_PI / 180);
	return new float[16]{ (float)0.5 / ang, 0, 0, 0,
						0, (float)0.5*a / ang, 0, 0,
						0, 0, -(float)(zMax + zMin) / (zMax - zMin), -1,
						0, 0, (float)(-2 * zMax*zMin) / (zMax - zMin), 0 };
}

void rotateZ(float *m, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	float mv0 = m[0], mv4 = m[4], mv8 = m[8];

	m[0] = c * m[0] - s * m[1];
	m[4] = c * m[4] - s * m[5];
	m[8] = c * m[8] - s * m[9];
	m[1] = c * m[1] + s * mv0;
	m[5] = c * m[5] + s * mv4;
	m[9] = c * m[9] + s * mv8;
}

// create the application here.
Application::Application()
{
}

// Initialize current game.
int Application::Init(const char *resPath)
{
	FileSystem::GetInstance()->Init(resPath);
	//creation of shaders and program 
	myShaders.Init("Shaders/Basic.vs", "Shaders/Basic.fs");
	/// Triangle
	vertexData[0].pos = Vector3(0, 0.5, 0);
	vertexData[1].pos = Vector3(-0.5, 0, 0);
	vertexData[2].pos = Vector3(0.5, 0, 0);

	vertexData[0].color = Vector3(1.0, 0.0, 0.0);
	vertexData[1].color = Vector3(0.0, 1.0, 0.0);
	vertexData[2].color = Vector3(0.0, 0.0, 1.0);

	proj_matrix = get_projection(40, 800 / 640, 1, 100);
	mov_matrix = new float[16]{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	view_matrix = new float[16]{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	view_matrix[14] = - 1.5 ;
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

	rotateZ(mov_matrix, 0.2 * deltatime);
}

// Render application

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(myShaders.program);

	if (myShaders.a_position != -1)
	{
		glEnableVertexAttribArray(myShaders.a_position);
		glVertexAttribPointer(myShaders.a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), vertexData);
	}

	if (myShaders.a_color != -1)
	{
		glEnableVertexAttribArray(myShaders.a_color);
		glVertexAttribPointer(myShaders.a_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (float*)vertexData+3);
	}

	if (myShaders.mMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.mMatrix, 1, GL_FALSE, mov_matrix);
		// 1 -> one matrix
	}
	if (myShaders.pMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.pMatrix, 1, GL_FALSE, proj_matrix);
	}
	if (myShaders.vMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.vMatrix, 1, GL_FALSE, view_matrix);
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);
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
