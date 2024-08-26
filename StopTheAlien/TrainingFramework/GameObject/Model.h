#pragma once
#include<GLES2/gl2.h>
#include<string>
class Model {

	public:
		Model();
		~Model();
		Model(const char* strModelFileName);

		GLuint getVboId();
		GLuint getIboId();
		GLuint getNumIndices();
		std::string GetModelID();
		void SetModelID(std::string modelID);

	private:
		std::string m_modelID;
		GLuint m_vboId;
		GLuint m_iboId;
		GLuint m_numIndices;
};

