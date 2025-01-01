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

	m_overBackground = SceneManager::GetInstance()->GetObjectByID("over_background");
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_back_to_menu"));
	m_text.SetPos({ 640, 190, 1 }); // Đặt vị trí của văn bản
	m_text.GetTextColor() = SDL_Color({ 255, 255, 255, 255 }); // Đặt màu sắc của văn bản
	m_text.GetTextSize() = 80; // Đặt kích thước của văn bản
	m_text.GetTextMessage() = "CONGRATULATION!"; // Đặt nội dung văn bản
	m_text.Init("../Resources/Fonts/orbitron/Orbitron-Black.ttf");
	AddSoundByName("over");
	PlaySoundByName("over", 9, -1);

	AddText("over_scores");

	std::set<int> uniqueIndices;	
	while (uniqueIndices.size() < 3) {
		int randomIndex = rand() % 6;
		uniqueIndices.insert(randomIndex);		
	}
	std::vector<int> indices(uniqueIndices.begin(), uniqueIndices.end());

	for (int i = 0; i < 3; i++) {
		auto button = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
		button->Set2DPos(640, 400 + i * 100);
		button->SetSize(250, 70);
		button->SetTexture(buttonTexture[indices[i]].c_str());
		m_choice.push_back(button);
	}
}

void GSOver::Exit() {
	StopSoundByName("over", 9);
}

void GSOver::Pause() {
	PauseSoundByName("over");
}

void GSOver::Resume() {
	ResumeSoundByName("over");
}

void GSOver::Update(float deltaTime) {
	UpdateText("over_scores", "Continue learning?", deltaTime);
}

void GSOver::Draw() {
	m_overBackground->Draw();
	m_text.Draw();
	for (auto& button : m_buttonList)
		button->Draw();
	RenderText("over_scores");
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