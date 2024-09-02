#pragma once
#include<GLES2/gl2.h>
#include<string>
class Texture {
public:
	Texture();
	~Texture();
	Texture(const char* strTextureFileName);
	GLuint getTextureId();
	void SetTextureID(std::string textureID);
	std::string GetID();
	GLuint m_textureID;
protected:
private:
	std::string m_ID;

};

