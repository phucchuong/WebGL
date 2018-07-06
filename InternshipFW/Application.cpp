#include "Application.h"
#include "FileSystem.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Input.h"

# define M_PI           3.14159265358979323846
Shaders myShaders;
GLuint VertexDataBuffer, IndicesBuffer, textureBuffer;
GLint n;

bool LoadOBJ(const char* path, std::vector<Vector3> &out_vertex, std::vector<Vector2> &out_texcoord, std::vector<Vector3> &out_normals) {
	FILE *filein;
	filein = fopen(path, "r");
	if (!filein) {
		return false;
	}

	std::vector<int> vertexIndices, texcoordIndices, normalIndices;
	std::vector<Vector3> temp_vertices, temp_normals;
	std::vector<Vector2> temp_texcoords;

	while (1) {
		char s[16];
		if (fscanf(filein, "%s", s) == EOF) {
			break;
		}
		if (strcmp(s, "v") == 0) {
			Vector3 vertex;
			fscanf(filein, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		if (strcmp(s, "vt")==0) {
			Vector2 texcoord;
			float x;
			fscanf(filein, "%f %f %f\n", &texcoord.x, &texcoord.y,&x);
			temp_texcoords.push_back(texcoord);
		}
		if (strcmp(s, "vn")==0) {
			Vector3 normal;
			fscanf(filein, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		if (strcmp(s, "f")==0) {
			int vertexIndex[3], texcoordIndex[3], normalIndex[3];
			int check = fscanf(filein, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &texcoordIndex[0], &normalIndex[0],
				&vertexIndex[1], &texcoordIndex[1], &normalIndex[1], &vertexIndex[2], &texcoordIndex[2], &normalIndex[2]);
			if (check != 9) {
				printf("wrong form !\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			texcoordIndices.push_back(texcoordIndex[0]);
			texcoordIndices.push_back(texcoordIndex[1]);
			texcoordIndices.push_back(texcoordIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		int vertexIndex = vertexIndices[i];
		Vector3 vertex = temp_vertices[vertexIndex - 1];
		out_vertex.push_back(vertex);

		int texcoordIndex = texcoordIndices[i];
		Vector2 texcoord = temp_texcoords[texcoordIndex - 1];
		out_texcoord.push_back(texcoord);

		int normalIndex = normalIndices[i];
		Vector3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}

	for (int i = 0; i < temp_vertices.size(); i++) {
		Vector3 temp = temp_vertices[i];
		printf("%f %f %f\n", temp.x, temp.y, temp.z);
	}
	return true;
}

float * proj_matrix, *mov_matrix, *view_matrix;

float* get_projection(float angle, float a, float zMin, float zMax) {
	float ang = (float)tanf((angle*0.5)*M_PI / 180);
	return new float[16]{ (float)0.5 / ang, 0, 0, 0,
						0, (float)0.5*a / ang, 0, 0,
						0, 0, -(float)(zMax + zMin) / (zMax - zMin), -1,
						0, 0, (float)(-2 * zMax*zMin) / (zMax - zMin), 0 };
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


// create the application here.
Application::Application()
{
}
GLint width, height, bpp;
// Initialize current game.
int Application::Init(const char *resPath)
{
	FileSystem::GetInstance()->Init(resPath);
	//creation of shaders and program 
	myShaders.Init("Shaders/Basic.vs", "Shaders/Basic.fs");

	std::vector<Vector3> vertexData;
	std::vector<Vector2> texcoordData;
	std::vector<Vector3> normalData;
	LoadOBJ("betty.obj", vertexData, texcoordData, normalData);
	n = vertexData.size();
	/// buffer
	glGenBuffers(1, &VertexDataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexDataBuffer);
	glBufferData(GL_ARRAY_BUFFER, n*sizeof(Vector3), &vertexData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);	

	/*glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
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
	glBindTexture(GL_TEXTURE_2D, NULL);*/

	proj_matrix = get_projection(40, 800 / 640, 1, 100);
	mov_matrix = new float[16]{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	view_matrix = new float[16]{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	view_matrix[14] = view_matrix[14] - 3;

	vertexData.clear();
	return true;
}

// Update current state
void Application::Update(float deltatime)
{
	Input * input = Input::GetInstance();
	rotateY(mov_matrix, deltatime);
}

// Render application

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 0);

	glUseProgram(myShaders.program);

	if (myShaders.a_position != -1)
	{
		glEnableVertexAttribArray(myShaders.a_position);
		glBindBuffer(GL_ARRAY_BUFFER, VertexDataBuffer);
		glVertexAttribPointer(myShaders.a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
	}
	/*if (myShaders.a_texcoord != -1)
	{
		glEnableVertexAttribArray(myShaders.a_texcoord);
		glBindBuffer(GL_ARRAY_BUFFER, VertexDataBuffer);
		glVertexAttribPointer(myShaders.a_texcoord, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)+12);
	}
	if (myShaders.a_texture != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBuffer);
		glUniform1i(myShaders.a_texture, 0);
	}*/
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
	
	/*glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, IndicesBuffer);*/
	
	glBindBuffer(GL_ARRAY_BUFFER, VertexDataBuffer);
	glDrawArrays(GL_TRIANGLES, 0, n );
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
