#pragma once
#include "../TrainingFramework/stdafx.h"
#include <string>
#include <vector>
#include <memory>
#include "ResourceManager.h"
#include "../TrainingFramework/GameObject/Object.h"
#include "../TrainingFramework/GameObject/Camera.h"
#include "../TrainingFramework/GameObject/Animation.h"
#include "../TrainingFramework/GameObject/GameButton.h"
#include "../BaseAlien.h"
#include "../Bullet.h"
#include "SingletonClass.h"
#include "../TrainingFramework/Globals.h"

class SceneManager : public SingletonClass<SceneManager> {
public:
    SceneManager();
    ~SceneManager();
    
    void LoadScene(char* smFile);

    std::shared_ptr<Object> GetObjectByID(const char* objectID);
    std::shared_ptr<GameButton> GetButtonByID(const char* buttonID);
    std::shared_ptr<Animation> GetAnimationByID(const char* animationID);
    std::shared_ptr<BaseAlien> GetAlienByID(const char* alienID);
    std::shared_ptr<Camera> GetCamera(const char* cameraID);

private:
    std::map<std::string, std::shared_ptr<Object>> m_objects;
    std::map<std::string, std::shared_ptr<GameButton>> m_buttons;
    std::map<std::string, std::shared_ptr<Animation>> m_animations;
    std::map<std::string, std::shared_ptr<BaseAlien>> m_aliens;
    std::shared_ptr<Camera> m_camera;
};

