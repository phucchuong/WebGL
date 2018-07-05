#include "Application.h"
#include "FileSystem.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Input.h"

Shaders myShaders;

Vertex vertexData[3];

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
	
	vertexData[0].pos = Vector3( 0, 0.5, 0 );
	vertexData[1].pos = Vector3( -0.5, -0.5, 0 );
	vertexData[2].pos = Vector3( 0.5, -0.5, 0 );


	return true;
}

// Update current state
void Application::Update(float deltatime)
{
	Input * input = Input::GetInstance();	
	
	if ( input->HasPointer(POINTER_DRAGGED) )
	{
		LOGI("\nPOINTER_DRAGGED");
	}
	if ( input->HasPointer(POINTER_DOWN) )
	{
		LOGI("\nPOINTER_DOWN");
	}
	if ( input->HasPointer(POINTER_UP) )
	{
		LOGI("\nPOINTER_UP");
	}
	
	if ( input->KeyPressed() )
	{
		LOGI("\nKey Pressed");
	}
}

// Render application

void Application::Render()
{	
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaders.program);	


	if(myShaders.a_position != -1)
	{
		glEnableVertexAttribArray(myShaders.a_position);
		glVertexAttribPointer(myShaders.a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), vertexData);
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

