#include "../TrainingFramework/stdafx.h"
#include "Texture.h"
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "../Utilities/stb_image.h"
#include "../Utilities/TGA.h"

Texture::Texture() 
{
}

Texture::~Texture() 
{
}

Texture::Texture(const char* strTextureFileName) {
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	std::string fileName(strTextureFileName);
	std::string stringPath = "../Resources/Textures/" + fileName + ".png";

	//Texture
	int width, height, bpp;
	//stbi_set_flip_horizontal_on_load(true);
	unsigned char* image = stbi_load(stringPath.c_str(), &width, &height, &bpp, 0);
	bpp *= 8;

	if (bpp == 24)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (bpp == 32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	// Cài đặt wrapping, filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (image)	
	{
		free(image);
	}
}

GLuint Texture::getTextureId() {
	return m_textureID;
}

void Texture::SetTextureID(std::string textureID) 
{
	m_ID = textureID;
};
std::string Texture::GetID()
{
	return m_ID;
};