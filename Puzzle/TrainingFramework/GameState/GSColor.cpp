#include "../TrainingFramework/stdafx.h"
#include "GSColor.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSColor::GSColor() : index(0), isCorrect(false)
{
	m_stateType = STATE_PUZZLE;		
}

GSColor::~GSColor()
{
}

void GSColor::Init()
{
	srand(static_cast<unsigned>(time(0)));

	auto sceneManager = SceneManager::GetInstance();
	m_objectVector.push_back(sceneManager->GetObjectByID("zoo_background"));

	m_buttonList = {
		sceneManager->GetButtonByID("button_pause")
	};

	m_pauseButtonList = {
		sceneManager->GetButtonByID("button_resume_2"),
		sceneManager->GetButtonByID("button_home"),
		sceneManager->GetButtonByID("button_tutorial")
	};

	std::vector<int> uniqueIndices;
	while (uniqueIndices.size() < 6) {
		int randomIndex = rand() % color.size();
		if (std::find(uniqueIndices.begin(), uniqueIndices.end(), randomIndex) == uniqueIndices.end()) {
			uniqueIndices.push_back(randomIndex);
		}
	}

	for (int i = 0; i < 6; i++) {
		std::string fileName = color[uniqueIndices[i]];
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(390 + i * 100, 300);
		slot->SetSize(100, 100);
		m_choice.push_back(slot);
	}

	for (int i = 0; i < 6; i++) {
		std::string fileName = color[uniqueIndices[i]];
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(390 + i * 100, 600);
		slot->SetSize(100, 100);
		m_question.push_back(slot);
	}

	/*for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			int index = i * 2 + j; 
			if (index < uniqueIndices.size()) {
				std::string fileName = color[uniqueIndices[index]];
				auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
				slot->Set2DPos(400 + i * 240, 600 + j * 200);
				slot->SetSize(200, 200);
				m_question.push_back(slot);
			}
		}
	}*/


	NewQuestion();	
	UpdateChoiceObjects();	

	AddSoundByName("play");

	AddSoundByName("correct");
	PlaySoundByName("play", 7, -1);
}

void GSColor::Exit()
{
	StopSoundByName("play", 7);
}

void GSColor::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSColor::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSColor::Update(float deltaTime) {
	
}

void GSColor::Draw(){
	DrawVectorObject(m_objectVector);
	DrawVectorObject(m_question);

	for (auto& button : m_buttonList)
		button->Draw();

	DrawVectorObject(m_choice);	

	if (!GSMachine::GetInstance()->IsRunning())	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto &button : m_pauseButtonList)
			button->Draw();
	}	
}

void GSColor::HandleEvents()
{
}

void GSColor::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSColor::HandleTouchEvents(float x, float y, bool bIsPressed) {
	if (GSMachine::GetInstance()->IsRunning()) {
		for (auto& button : m_buttonList) {
			if (button->HandleTouchEvent(x, y, bIsPressed)) {
				switch (button->m_type) {
				case BUTTON_PAUSE:
					GSMachine::GetInstance()->Pause();
					button->SetAlpha(0.5f);
					break;
				};
			}
		}
		
		for (int i = 0; i < m_choice.size(); i++) {
			if (m_choice[i]->HandleTouchEvent(x, y, bIsPressed)) {
				if (i==index) {
					PlaySoundByName("correct", 8, 0);
					printf("Correct\n");
					key.erase(i, 1);
					isCorrect = true;
					return;
				}					
			}
		}
	}
	else
	{
		for (auto& button : m_pauseButtonList)
			if (button->HandleTouchEvent(x, y, bIsPressed))
			{
				switch (button->m_type)
				{
				case BUTTON_RESUME:
					GSMachine::GetInstance()->Resume();
					break;
				case BUTTON_BACK_TO_MENU:
					GSMachine::GetInstance()->PopState();
					break;
				case BUTTON_TUTORIAL:
					GSMachine::GetInstance()->Resume();
					GSMachine::GetInstance()->PushState(STATE_TUTORIAL);
					break;
				}
			};
	}
}

void GSColor::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSColor::UpdateChoiceObjects() {
	
}

void GSColor::NewQuestion() {
	
}