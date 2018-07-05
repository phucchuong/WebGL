#include "Application.h"
#include "FileSystem.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Input.h"

# define M_PI           3.14159265358979323846
Shaders myShaders;
///cube
Vertex vertexData[24];

unsigned short indices[36] = { 0,3,1, 1,3,2,  4,5,7, 7,5,6,
8,10,9, 8,11,10,  12,13,14, 12,14,15,
16,18,17, 18,16,19,  20,21,22, 20,22,23 };
float * proj_matrix, *mov_matrix, *view_matrix;

float* get_projection(float angle, float a, float zMin, float zMax) {
	float ang = (float)tanf((angle*0.5)*M_PI / 180);
	return new float[16]{ (float)0.5 / ang, 0, 0, 0,
						0, (float)0.5*a / ang, 0, 0,
						0, 0, -(float)(zMax + zMin) / (zMax - zMin), -1,
						0, 0, (float)(-2 * zMax*zMin) / (zMax - zMin), 0 };
}

void rotateX(float *m, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	float mv1 = m[1], mv5 = m[5], mv9 = m[9];

	m[1] = m[1] * c - m[2] * s;
	m[5] = m[5] * c - m[6] * s;
	m[9] = m[9] * c - m[10] * s;

	m[2] = m[2] * c + mv1 * s;
	m[6] = m[6] * c + mv5 * s;
	m[10] = m[10] * c + mv9 * s;
}

void rotateY(float *m, float angle) {
	float c = cos(angle);
	float s = sin(angle);
	float mv0 = m[0], mv4 = m[4], mv8 = m[8];

	m[0] = c * m[0] + s * m[2];
	m[4] = c * m[4] + s * m[6];
	m[8] = c * m[8] + s * m[10];

	m[2] = c * m[2] - s * mv0;
	m[6] = c * m[6] - s * mv4;
	m[10] = c * m[10] - s * mv8;
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

	/// cube
	//vertex
	{vertexData[0].pos = Vector3(-1, -1, -1);
	vertexData[1].pos = Vector3(1, -1, -1);
	vertexData[2].pos = Vector3(1, 1, -1);
	vertexData[3].pos = Vector3(-1, 1, -1);

	vertexData[4].pos = Vector3(-1, -1, 1);
	vertexData[5].pos = Vector3(1, -1, 1);
	vertexData[6].pos = Vector3(1, 1, 1);
	vertexData[7].pos = Vector3(-1, 1, 1);

	vertexData[8].pos = Vector3(-1, -1, -1);
	vertexData[9].pos = Vector3(-1, 1, -1);
	vertexData[10].pos = Vector3(-1, 1, 1);
	vertexData[11].pos = Vector3(-1, -1, 1);

	vertexData[12].pos = Vector3(1, -1, -1);
	vertexData[13].pos = Vector3(1, 1, -1);
	vertexData[14].pos = Vector3(1, 1, 1);
	vertexData[15].pos = Vector3(1, -1, 1);

	vertexData[16].pos = Vector3(-1, -1, -1);
	vertexData[17].pos = Vector3(-1, -1, 1);
	vertexData[18].pos = Vector3(1, -1, 1);
	vertexData[19].pos = Vector3(1, -1, -1);

	vertexData[20].pos = Vector3(-1, 1, -1);
	vertexData[21].pos = Vector3(-1, 1, 1);
	vertexData[22].pos = Vector3(1, 1, 1);
	vertexData[23].pos = Vector3(1, 1, -1); }
	//color
	{
		vertexData[0].color = Vector3(1, 0, 1);
		vertexData[1].color = Vector3(1, 0, 1);
		vertexData[2].color = Vector3(1, 0, 1);
		vertexData[3].color = Vector3(1, 0, 1);

		vertexData[4].color = Vector3(1, 1, 1);
		vertexData[5].color = Vector3(1, 1, 1);
		vertexData[6].color = Vector3(1, 1, 1);
		vertexData[7].color = Vector3(1, 1, 1);

		vertexData[8].color = Vector3(0, 0, 1);
		vertexData[9].color = Vector3(0, 0, 1);
		vertexData[10].color = Vector3(0, 0, 1);
		vertexData[11].color = Vector3(0, 0, 1);

		vertexData[12].color = Vector3(1, 0, 0);
		vertexData[13].color = Vector3(1, 0, 0);
		vertexData[14].color = Vector3(1, 0, 0);
		vertexData[15].color = Vector3(1, 0, 0);

		vertexData[16].color = Vector3(1, 1, 0);
		vertexData[17].color = Vector3(1, 1, 0);
		vertexData[18].color = Vector3(1, 1, 0);
		vertexData[19].color = Vector3(1, 1, 0);

		vertexData[20].color = Vector3(0, 1, 0);
		vertexData[21].color = Vector3(0, 1, 0);
		vertexData[22].color = Vector3(0, 1, 0);
		vertexData[23].color = Vector3(0, 1, 0);
	}

	proj_matrix = get_projection(40, 800 / 640, 1, 100);
	mov_matrix = new float[16]{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	view_matrix = new float[16]{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	view_matrix[14] = view_matrix[14] - 6;
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
	rotateX(mov_matrix, 0.3*deltatime);
	rotateY(mov_matrix, 0.6*deltatime);
	rotateZ(mov_matrix, 0.0 * deltatime);
}

// Render application

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(myShaders.program);

	if (myShaders.a_position != -1)
	{
		glEnableVertexAttribArray(myShaders.a_position);
		glVertexAttribPointer(myShaders.a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), vertexData);
	}
	if (myShaders.a_color != -1)
	{
		glEnableVertexAttribArray(myShaders.a_color);
		glVertexAttribPointer(myShaders.a_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)vertexData+12);
	}

	if (myShaders.mMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.mMatrix, 1, GL_FALSE, mov_matrix);
	}
	if (myShaders.pMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.pMatrix, 1, GL_FALSE, proj_matrix);
	}
	if (myShaders.vMatrix != -1)
	{
		glUniformMatrix4fv(myShaders.vMatrix, 1, GL_FALSE, view_matrix);
	}

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);
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
