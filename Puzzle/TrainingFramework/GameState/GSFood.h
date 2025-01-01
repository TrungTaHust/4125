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

class GSFood :
	public GSBase
{
public:
	GSFood();
	~GSFood();

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

	void	NewQuestion();
	void	NewFruit();
private:
	std::vector<std::shared_ptr<Object>> m_objectVector;
	std::vector<std::shared_ptr<GameButton>> m_buttonList;
	std::vector<std::shared_ptr<GameButton>> m_pauseButtonList;
	std::vector<std::shared_ptr<Object>> m_falling;
	std::shared_ptr<Object> m_basket;
	std::vector<std::shared_ptr<Object>> m_textAnswer;
	
	std::shared_ptr<Object>m_question;
	std::vector<std::shared_ptr<Object>> m_choice;
	std::string key;
	int index;
	float m_time;
	float end_time = 2;
	bool isCorrect;
	bool isCompleted = false;
	int count = 0;
	int score = 0;

	std::shared_ptr<Object>m_scoreFrame;

	std::vector<char> nonNameChars;
	std::vector<char> nameChars;

	std::vector<std::string> fruits = {
		"apple", "avocado", "grape", "lemon", "mango", "orange",
		"pineapple", "watermelon"
	};	
};