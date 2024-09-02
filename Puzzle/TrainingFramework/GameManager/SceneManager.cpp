#include "../TrainingFramework/stdafx.h"
#include "SceneManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::LoadScene(char* smFile) {
    std::ifstream file(smFile);
    if (!file.is_open()) {
        std::cout << "Could not open the file.\n";
        return;
    }
    
    std::string type;

    while (file >> type) {
        if (type.find("Objects") != std::string::npos) {
            int numModels;
            file >> numModels;
            for (int i = 0; i < numModels; i++) {
                std::string object_id;
                std::string model_id;
                std::string texture_id;
                std::string shader_id;
                Vector2 pos;
                Vector3 rot;
                Vector2 size;

                file >> type >> object_id >> type >> model_id >> type >> texture_id >> type >> shader_id;

                file >> type >>
                    pos.x >> pos.y >> type >>
                    rot.x >> rot.y >> rot.z >> type >>
                    size.x >> size.y >> type;
                // Lưu thông tin về object vào đây

                std::shared_ptr<Object> newObject = std::make_shared<Object>(model_id.c_str(), texture_id.c_str(), shader_id.c_str());
                newObject->Set2DPos(pos.x, pos.y);
                newObject->SetRotate(rot);
                newObject->SetSize(size.x, size.y);
                newObject->SetObjectID(object_id);
                m_objects.insert(std::make_pair(object_id, newObject));
            }
        }
        else if (type.find("Button") != std::string::npos) {
            int numModels;
            file >> numModels;
            for (int i = 0; i < numModels; i++) {
                std::string button_id;
                std::string model_id;
                std::string texture_id;
                std::string shader_id;
                std::string buttonType;
                Vector2 pos;
                Vector3 rot;
                Vector2 size;

                file >> type >> button_id >> type >> buttonType >> type >> model_id >> type >> texture_id >> type >> shader_id;

                file >> type >>
                    pos.x >> pos.y >> type >>
                    rot.x >> rot.y >> rot.z >> type >>
                    size.x >> size.y >> type;
                // Lưu thông tin về object vào đây
                
                std::shared_ptr<GameButton> newButton = std::make_shared<GameButton>(model_id.c_str(), texture_id.c_str(), shader_id.c_str(), buttonType.c_str());
                newButton->Set2DPos(pos.x, pos.y);
                newButton->SetRotate(rot);
                newButton->SetSize(size.x, size.y);
                newButton->SetObjectID(button_id);
                m_buttons.insert(std::make_pair(button_id, newButton));
            }
        }
        else if (type.find("CAMERA") != std::string::npos) {
            float nearInfo, farInfo, fov, speed;
            file >> type >> nearInfo >> type >> farInfo >> type >> fov >> type >> speed;
            m_camera = std::make_shared<Camera>(nearInfo, farInfo, fov, speed);
        }
        else if (type.find("Animation") != std::string::npos) {
            int numAnimations;
            file >> numAnimations;
            for (int i = 0; i < numAnimations; i++) {
                std::string animation_id;
                std::string model_id;
                int numTexture;
                std::vector<std::string> textureIds;
                std::vector<GLint> numFrames, numAction, currentAction;
                std::vector<GLfloat> frameTime;
                std::string shader_id;
                Vector2 pos;
                Vector3 rot;
                Vector2 size;

                file >> type >> animation_id >> type >> model_id >> type >> numTexture;
                for (int j = 0; j < numTexture; ++j) {
                    std::string textureId;
                    GLint frame, numAct, curAct;
                    GLfloat frTime;
                    file >> type >> textureId;
                    textureIds.push_back(textureId);
                    file >> type >> frame >> type >> numAct >> type >> curAct >> type >> frTime;
                    numFrames.push_back(frame);
                    numAction.push_back(numAct);
                    currentAction.push_back(curAct);
                    frameTime.push_back(frTime);
                }

                file >> type >> shader_id >> type >>
                    pos.x >> pos.y >> type >>
                    rot.x >> rot.y >> rot.z >> type >>
                    size.x >> size.y >> type;
                // Lưu thông tin về animation vào đây
                std::shared_ptr<Model> modelPtr = ResourceManager::GetInstance()->GetModelPointerByName(model_id.c_str());
                std::shared_ptr<Shaders> shaderPtr = ResourceManager::GetInstance()->GetShaderPointerByName(shader_id.c_str());
                std::vector<std::shared_ptr<Texture>> textureVector;
                std::shared_ptr<Texture> texturePtr = std::make_shared<Texture>();
                for (int j = 0; j < numTexture; ++j) {
                    texturePtr = ResourceManager::GetInstance()->GetTexturePointerByName(textureIds[j].c_str());
                    textureVector.push_back(texturePtr);
                }

                std::shared_ptr<Animation> newAnimation = std::make_shared<Animation>(modelPtr, textureVector, shaderPtr, numFrames, numAction, currentAction, frameTime);
                newAnimation->Set2DPos(pos.x, pos.y);
                newAnimation->SetRotate(rot);
                newAnimation->SetSize(size.x, size.y);
                newAnimation->SetObjectID(animation_id);
                m_animations.insert(std::make_pair(animation_id, newAnimation));
            }
        }
        else if (type.find("Alien") != std::string::npos) {
            int numAnimations;
            file >> numAnimations;
            for (int i = 0; i < numAnimations; i++) {
                std::string alien_id;
                std::string model_id;
                int numTexture;
                std::vector<std::string> textureIds;
                std::vector<GLint> numFrames, numAction, currentAction;
                std::vector<GLfloat> frameTime;
                std::string shader_id;
                Vector2 size;
                float speed;
                int health;
                int score;
                int coin;

                file >> type >> alien_id >> type >> model_id >> type >> numTexture;
                for (int j = 0; j < numTexture; ++j) {
                    std::string textureId;
                    GLint frame, numAct, curAct;
                    GLfloat frTime;
                    file >> type >> textureId;
                    textureIds.push_back(textureId);
                    file >> type >> frame >> type >> numAct >> type >> curAct >> type >> frTime;
                    numFrames.push_back(frame);
                    numAction.push_back(numAct);
                    currentAction.push_back(curAct);
                    frameTime.push_back(frTime);
                }

                file >> type >> shader_id >> type >> size.x >> size.y 
                    >> type >> speed >> type >> health >> type >> score >> type >> coin >> type; //Buffer ở cuối này là dòng gạch ngang trong file
                // Lưu thông tin về alien vào đây
                std::shared_ptr<Model> modelPtr = ResourceManager::GetInstance()->GetModelPointerByName(model_id.c_str());
                std::shared_ptr<Shaders> shaderPtr = ResourceManager::GetInstance()->GetShaderPointerByName(shader_id.c_str());
                std::vector<std::shared_ptr<Texture>> textureVector;
                std::shared_ptr<Texture> texturePtr = std::make_shared<Texture>();
                for (int j = 0; j < numTexture; ++j) {
                    texturePtr = ResourceManager::GetInstance()->GetTexturePointerByName(textureIds[j].c_str());
                    textureVector.push_back(texturePtr);
                }

                std::shared_ptr<BaseAlien> newAlien = std::make_shared<BaseAlien>(modelPtr, textureVector, shaderPtr, numFrames, numAction, currentAction, frameTime, speed, health, score);
                newAlien->SetSize(size.x, size.y);
                newAlien->SetCoin(coin);
                newAlien->SetObjectID(alien_id);
                m_aliens.insert(std::make_pair(alien_id, newAlien));
            }
            } 
    }
    file.close();
}

std::shared_ptr<Object> SceneManager::GetObjectByID(const char* objectID) 
{
    for (auto& it : m_objects)
    {
        if (it.first == objectID)
        {
            return it.second;
        }
    }
    return nullptr;
}

std::shared_ptr<Animation> SceneManager::GetAnimationByID(const char* animationID)
{
    for (auto& it : m_animations)
    {
        if (it.first == animationID)
        {
            return it.second;
        }
    }
    return nullptr;
}

std::shared_ptr<BaseAlien> SceneManager::GetAlienByID(const char* alienID)
{
    for (auto& it : m_aliens)
    {
        if (it.first == alienID)
        {
            return it.second;
        }
    }
    return nullptr;
}

std::shared_ptr<GameButton> SceneManager::GetButtonByID(const char* buttonID)
{
    for (auto& it : m_buttons)
    {
        if (it.first == buttonID)
        {
            return it.second;
        }
    }
    return nullptr;
}

std::shared_ptr<Camera> SceneManager::GetCamera(const char* cameraID) {
    /*
    if (cameraID == "2D") 
    {
        return m_camera2D;
    }

    else if (cameraID == "3D") 
    {
        return m_camera3D;
    }

    else
    {
    */
        return m_camera;
    //}
}