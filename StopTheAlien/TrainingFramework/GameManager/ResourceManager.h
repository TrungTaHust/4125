#pragma once
#include "../TrainingFramework/stdafx.h"
#include "SingletonClass.h"
#include "../TrainingFramework/GameObject/Shaders.h"
#include "../TrainingFramework/GameObject/Texture.h"
#include "../TrainingFramework/GameObject/Model.h"
#include<string>
#include<map>
#include "../TrainingFramework/GameObject/Camera.h"
#include "../TrainingFramework/GameObject/Sound.h"
#include "../TrainingFramework/GameObject/Text.h"

class ResourceManager : public SingletonClass<ResourceManager> {
public:
    
    ResourceManager();
    ~ResourceManager();

    void LoadResource(const char* rmFile);

    std::shared_ptr<Shaders> GetShaderPointerByName(const char* shaderName);
    std::shared_ptr<Texture> GetTexturePointerByName(const char* textureName);
    std::shared_ptr<Model> GetModelPointerByName(const char* modelName);
    std::shared_ptr<Sound> GetSoundPointerByName(const char* soundName);
    std::shared_ptr<Text> GetTextPointerByName(const char* name);

private:
    
    std::map<std::string, std::shared_ptr<Shaders>> m_shaders;
    std::map<std::string, std::shared_ptr<Texture>> m_textures;
    std::map<std::string, std::shared_ptr<Model>> m_models;
    std::map<std::string, std::shared_ptr<Sound>> m_sounds;
    std::map<std::string, std::shared_ptr<Text>> m_texts;

};

