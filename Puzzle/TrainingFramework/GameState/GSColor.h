#pragma once
#include "GSBase.h"
#include "../TrainingFramework/GameObject/Object.h"
#include "../TrainingFramework/GameObject/Animation.h"
#include "../TrainingFramework/GameObject/GameButton.h"
#include "../TrainingFramework/GameObject/Text.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include <cstdlib>

class GSColor :
	public GSBase
{
public:
	GSColor();
	~GSColor();

	void	Init() override;
	void	Exit() override;
	void	Pause() override;
	void	Resume() override;
	void	Update(float deltaTime) override;
	void	Draw() override;

	void	HandleEvents() override;
	void	HandleKeyEvents(int key, bool bIsPressed) override;
	void	HandleTouchEvents(float x, float y, bool bIsPressed) override;
	void	HandleMouseMoveEvents(float x, float y) override;

	void	UpdateChoiceObjects();
	void	NewQuestion();
private:
	std::vector<std::shared_ptr<Object>> m_objectVector;
	std::vector<std::shared_ptr<GameButton>> m_buttonList;
	std::vector<std::shared_ptr<GameButton>> m_pauseButtonList;
	std::vector<std::shared_ptr<Object>> m_choice;
	std::vector<std::shared_ptr<Object>> m_question;

	std::string key;
	int index;
	int m_time = 1;
	bool isCorrect;

	std::vector<std::string> color = {
		"black_vec", "blue_vec", "brown_vec", "cyan_vec", 
		"dark_blue_vec", "green_vec", "grey_vec", "lime_vec", 
		"orange_vec", "pink_vec", "purple_vec", "red_vec",
		"white_vec", "yellow_vec"
	};

	
};