#include "../TrainingFramework/stdafx.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "Text.h"
#include <iostream>

Text::Text()
{
    m_Text = "DefaultText";
    m_Color = { 255, 0, 0, 255 };
    m_Size = 24; // Kích thước mặc định

    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    auto rm = ResourceManager::GetInstance();
    m_shader = rm->GetShaderPointerByName("TriangleShader");
    m_model = rm->GetModelPointerByName("Sprite2D");
    m_texture = std::make_shared<Texture>();
}

Text::Text(const Text& other)
{
    m_Text = other.m_Text;
    m_Color = other.m_Color;
    m_Size = other.m_Size;
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    auto rm = ResourceManager::GetInstance();
    m_shader = rm->GetShaderPointerByName("TriangleShader");
    m_model = rm->GetModelPointerByName("Sprite2D");
    m_texture = std::make_shared<Texture>();
    m_font = other.m_font;
}

Text::Text(Vector2 pos, Vector4 color, GLint size, std::string message, std::string font)
{
    auto rm = ResourceManager::GetInstance();
    m_shader = rm->GetShaderPointerByName("TriangleShader");
    m_model = rm->GetModelPointerByName("Sprite2D");
    m_texture = std::make_shared<Texture>();
    
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    Set2DPos(pos.x, pos.y);
    auto x = (Uint8)color.x;
    auto y = (Uint8)color.y;
    auto z = (Uint8)color.z;
    auto w = (Uint8)color.w;

    GetTextColor() = SDL_Color({ x, y, z, w });
    GetTextSize() = size;
    GetTextMessage() = message;
    m_font = font;
    Init(font);
}

Text::~Text()
{
    TTF_Quit();
}

void Text::SetTextColor(Vector4 color) 
{
    auto x = (Uint8)color.x;
    auto y = (Uint8)color.y;
    auto z = (Uint8)color.z;
    auto w = (Uint8)color.w;

    GetTextColor() = SDL_Color({ x, y, z, w });
}

void Text::SetTextMessage(const char* message) 
{
    m_Text = std::string(message);
}

int Text::Init(std::string path)
{
    // Load font từ đường dẫn và kích thước
    m_font = path;
    font = TTF_OpenFont(path.c_str(), m_Size);
    if (!font)
    {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return 0;
    }

    // Tạo SDL_Surface chứa văn bản
    sFont = TTF_RenderUTF8_Blended(font, m_Text.c_str(), m_Color);
    if (!sFont)
    {
        std::cout << "Failed to render text! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return 0;
    }

    SDL_Surface* convertedSurface = SDL_CreateRGBSurfaceWithFormat(0, sFont->w, sFont->h, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_BlitSurface(sFont, NULL, convertedSurface, NULL);

    // Chuyển đổi SDL_Surface thành texture (sử dụng OpenGL)
    glGenTextures(1, &m_TextTextureID);
    
    glBindTexture(GL_TEXTURE_2D, m_TextTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedSurface->w, convertedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedSurface->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
 
    SetSize(convertedSurface->w, convertedSurface->h);
    // Giải phóng bộ nhớ của SDL_Surface
    SDL_FreeSurface(sFont);
    SDL_FreeSurface(convertedSurface);

    // Đóng font sau khi đã tạo texture
    TTF_CloseFont(font);

    m_texture->m_textureID= m_TextTextureID;

    return 1;
}

void Text::Draw()
{
    Object::Draw();
}

void Text::Update(float deltaTime)
{
    Init(m_font);
}