#include "../TrainingFramework/stdafx.h"
#include "Shaders.h"
#include <string>

int Shaders::Init(const char * fileName){
	std::string name(fileName);
	std::string vsPath = "../Resources/Shaders/" + name + ".vs";
	std::string fsPath = "../Resources/Shaders/" + name + ".fs";
	vertexShader = esLoadShader(GL_VERTEX_SHADER, vsPath.c_str());

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fsPath.c_str());

	if ( fragmentShader == 0 )	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	return 0;
}

Shaders::~Shaders() {
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void Shaders::SetShaderID(std::string shaderID)
{
	m_shaderID = shaderID;
};
std::string Shaders::GetShaderID() 
{
	return m_shaderID;
};