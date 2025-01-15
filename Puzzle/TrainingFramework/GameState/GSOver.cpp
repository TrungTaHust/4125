#include "../TrainingFramework/stdafx.h"
#include "GSOver.h"
#include "../TrainingFramework/stdafx.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "../TrainingFramework/GameState/GSMachine.h"
#include "../TrainingFramework/GameObject/Text.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSOver::GSOver() {
	m_stateType = STATE_GAMEOVER;
}

GSOver::~GSOver() {
}

void GSOver::Init() {
	srand(static_cast<unsigned>(time(0)));

	m_overBackground.push_back(SceneManager::GetInstance()->GetObjectByID("over_background"));
	m_overBackground.push_back(SceneManager::GetInstance()->GetObjectByID("congrat"));

	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_back_to_menu"));	

	AddSoundByName("menu");
	PlaySoundByName("menu", 9, -1);

	std::set<int> uniqueIndices;	
	while (uniqueIndices.size() < 3) {
		int randomIndex = rand() % 6;
		uniqueIndices.insert(randomIndex);		
	}
	std::vector<int> indices(uniqueIndices.begin(), uniqueIndices.end());

	for (int i = 0; i < 3; i++) {
		auto button = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
		button->Set2DPos(300 + i * 340, 480);
		button->SetSize(300, 300);
		button->SetTexture(buttonTexture[indices[i]].c_str());
		m_choice.push_back(button);
	}
}

void GSOver::Exit() {
	StopSoundByName("menu", 9);
}

void GSOver::Pause() {
	PauseSoundByName("menu");
}

void GSOver::Resume() {
	ResumeSoundByName("menu");
}

void GSOver::Update(float deltaTime) {
}

void GSOver::Draw() {
	DrawVectorObject(m_overBackground);
	for (auto& button : m_buttonList)
		button->Draw();
	DrawVectorObject(m_choice);
}

void GSOver::HandleEvents() {
}

void GSOver::HandleKeyEvents(int key, bool bIsPressed) {
}

void GSOver::HandleTouchEvents(float x, float y, bool bIsPressed) {
	for (auto& button : m_buttonList) {
		if (button->HandleTouchEvent(x, y, bIsPressed)) {
			switch (button->m_type) {
			case BUTTON_BACK_TO_MENU:
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				break;
			}
		};
	}
	for (auto& button : m_choice) {
		if (button->HandleTouchEvent(x, y, bIsPressed)) {
			std::string str = button->getTexture()->GetID();
			if (str == "btn_action") {
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PushState(STATE_ACTION);
			}
			else if (str == "btn_animals1") {
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PushState(STATE_ANIM1);
			}
			else if (str == "btn_animals2") {
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PushState(STATE_ANIM2);
			}
			else if (str == "btn_food") {
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PushState(STATE_FOOD);
			}
			else if (str == "btn_number") {
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PushState(STATE_COUNT);
			}
			else if (str == "btn_vehicles") {
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PushState(STATE_COUNT2);
			}			
		};
	}
}

void GSOver::HandleMouseMoveEvents(float x, float y) {
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);
	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);
}