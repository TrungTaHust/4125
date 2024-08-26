#pragma once
#include "../Utilities/utilities.h"
#include<string>

class Shaders {
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	
	void SetShaderID(std::string shaderID);
	std::string GetShaderID();
		
	int Init(const char* fileName);
	~Shaders();
private:
	std::string m_shaderID;
};