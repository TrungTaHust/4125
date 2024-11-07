﻿#pragma once
#include "GSBase.h"
#include "../TrainingFramework/GameObject/Object.h"
#include "../TrainingFramework/GameObject/Animation.h"
#include "../TrainingFramework/GameObject/GameButton.h"
#include "../TrainingFramework/GameObject/Text.h"
#include "../BaseAlien.h"
#include "../Bullet.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include <cstdlib>

class GSWord :
	public GSBase
{
public:
	GSWord();
	~GSWord();

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
private:
	std::vector<std::shared_ptr<Object>> m_objectVector;
	std::vector<std::shared_ptr<GameButton>> m_buttonList;
	std::vector<std::shared_ptr<GameButton>> m_pauseButtonList;
	std::vector<std::shared_ptr<Object>> m_frame;
	std::vector<std::shared_ptr<Object>> m_ans;
	std::vector<std::shared_ptr<Object>> m_choice;

	std::shared_ptr<Object>m_question;
	std::string answer;
	std::string key;
	int keyInt;
	char c;
	int index;
	int m_time = 2;
	bool isCorrect = false;
	std::string check;	

	std::vector<std::string> animals = {
		"bear", "cat", "cow", "crab", "deer", "dog", "dolphin", "duck",
		"elephant", "fox", "giraffe", "hippo", "horse", "koala", "lion",
		"monkey", "mouse", "owl", "panda", "parrot", "pig", "rabbit",
		"rhino", "sheep", "tiger", "turtle", "zebra"
	};

	std::vector<std::string> fruits = {
		"apple", "avocado", "grape", "lemon", "mango", "orange",
		"pineapple", "watermelon"
	};	
};