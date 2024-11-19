#include "../TrainingFramework/stdafx.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ResourceManager::ResourceManager() 
{
}

ResourceManager::~ResourceManager() 
{
}

void ResourceManager::LoadResource(const char* rmFile)
{
    std::ifstream file(rmFile);
    if (!file.is_open())
    {
        std::cout << "Could not open the file.\n";
        return;
    }

    std::string type;
    while (file >> type) 
    {
        if (type.find("Model") != std::string::npos)
        {
            int numModels;
            file >> numModels;
            for (int i = 0; i < numModels; i++)
            {
                std::string id;
                std::string fileName;
                file >> type >> id >> type >> fileName;
                
                // Lưu thông tin về model vào đây
                std::shared_ptr<Model> newModel = std::make_shared<Model>(fileName.c_str());
                newModel->SetModelID(id);
                m_models.insert(std::make_pair(fileName, newModel));
             }
        }
        else if (type.find("Textures") != std::string::npos) 
        {
            int numTextures;
            file >> numTextures;

            for (int i = 0; i < numTextures; i++) 
            {
                std::string id;
                std::string fileName;
                file >> type >> id >> type >> fileName;
                std::cout << "Loading texture: " << fileName << "\n";
                
                // Lưu thông tin về texture vào đây
                std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(fileName.c_str());
                size_t pos = fileName.find('/');

                // Cắt chuỗi, giữ lại phần sau dấu '/'
                if (pos != std::string::npos) {
                    fileName = fileName.substr(pos + 1);
                }
                newTexture->SetTextureID(fileName);
                m_textures.insert(std::make_pair(fileName, newTexture));
            }
        }
        else if (type.find("Shaders") != std::string::npos) 
        {
            int numShaders;
            file >> numShaders;

            for (int i = 0; i < numShaders; i++) 
            {
                std::string id;
                std::string fileName;
                file >> type >> id >> type >> fileName;
                
                std::shared_ptr<Shaders> newShader = std::make_shared<Shaders>();
                newShader->Init(fileName.c_str());
                newShader->SetShaderID(id);
                m_shaders.insert(std::make_pair(fileName, newShader));
            }
        }
        else if (type.find("Sound") != std::string::npos)
        {
            int numSounds;
            file >> numSounds;
            for (int i = 0; i < numSounds; i++)
            {
                std::string fileName;
                file >> type >> fileName;

                // Lưu thông tin về model vào đây
                std::shared_ptr<Sound> newSound = std::make_shared<Sound>(fileName.c_str());
                m_sounds.insert(std::make_pair(fileName, newSound));
            }
        }
        else if (type.find("Text") != std::string::npos)
        {
            int numTexts;
            file >> numTexts;
            for (int i = 0; i < numTexts; i++)
            {
                std::string id;
                Vector2 pos;
                Vector4 color;
                GLint size;
                std::string message;
                std::string font;
                file >> type >> id >> type >> pos.x >> pos.y;
                file >> type >> color.x >> color.y >> color.z >> color.w;
                file >> type >> size >> type >> message >> type >> font;

                // Lưu thông tin về model vào đây
                std::shared_ptr<Text> newText = std::make_shared<Text>(pos, color, size, message, font);
                m_texts.insert(std::make_pair(id, newText));
            }
        }
    }
    file.close();
}

std::shared_ptr<Shaders> ResourceManager::GetShaderPointerByName(const char* shaderName)
{
    auto it = m_shaders.find(shaderName);
    if (it != m_shaders.end())
    {
        return it->second;
    }

    return nullptr;
};

std::shared_ptr<Texture> ResourceManager::GetTexturePointerByName(const char* textureName)
{
    auto it = m_textures.find(textureName);
    if (it != m_textures.end())
    {
        return it->second;
    }
    return nullptr;
};

std::shared_ptr<Model> ResourceManager::GetModelPointerByName(const char* modelName)
{
    auto it = m_models.find(modelName);
    if (it != m_models.end())
    {
        return it->second;
    }
    return nullptr;
};

std::shared_ptr<Sound> ResourceManager::GetSoundPointerByName(const char* soundName)
{
    auto it = m_sounds.find(soundName);
    if (it != m_sounds.end())
    {
        return it->second;
    }
    return nullptr;
};

std::shared_ptr<Text> ResourceManager::GetTextPointerByName(const char* name)
{
    auto it = m_texts.find(name);
    if (it != m_texts.end())
    {
        auto& textPointer = it->second;
        auto camera = SceneManager::GetInstance()->GetCamera("smth");
        textPointer->SetCamera(camera);
        return textPointer;
    }
    return nullptr;
};
