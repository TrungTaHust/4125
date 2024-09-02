#pragma once
#include "../TrainingFramework/stdafx.h"
#include <string>
#include <memory>
#include "Object.h"
#include "Shaders.h"
#include "Vertex.h"
#include <SDL.h>
#include <SDL_ttf.h>

class Text : public Object
{
public:
	Text();
	Text(const Text& other);
	Text(Vector2 pos, Vector4 color, GLint size, std::string message, std::string font);
	~Text();

	int Init(std::string path);
	void Draw();
	void Update(float deltaTime);

	std::string& GetTextMessage() { return m_Text; }
	int& GetTextSize() { return m_Size; }
	SDL_Color& GetTextColor() { return m_Color; }
	void SetTextMessage(const char* message);
	void SetTextColor(Vector4 color);

	bool isUpdated;
private:
	std::string m_Text;
	SDL_Color m_Color = SDL_Color({0,0,0,255});
	int m_Size;
	std::string m_font;

	TTF_Font* font = nullptr;
	SDL_Surface* sFont = nullptr;
	GLuint m_TextTextureID;
};

