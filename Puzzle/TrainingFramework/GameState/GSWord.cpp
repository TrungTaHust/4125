#include "../TrainingFramework/stdafx.h"
#include "GSWord.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSWord::GSWord()
	: answer("     "), check("00000")
{
	m_stateType = STATE_PUZZLE;	
}

GSWord::~GSWord()
{
}

void GSWord::Init()
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

	key = animals[rand() % animals.size()];

	m_question = std::make_shared<Object>("Sprite2D", key.c_str(), "TriangleShader");
	m_question->Set2DPos(640, 200);
	m_question->SetSize(200, 200);

	c = rand() % 26 + 'a';
	index = rand() % key.size();
	key.insert(index,1,c);

	int totalWidth = key.size() * 50 + (key.size() - 1) * 30;
	int leftAlign = (1280 - totalWidth) / 2;

	for (int i = 0; i < key.size(); i++) {
		std::string fileName = std::string(1, key[i]) + "02";
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(leftAlign + i * 80, 400);
		slot->SetSize(50, 50);
		m_choice.push_back(slot);		
	}	
	

	m_objectVector.push_back(m_question);
	AddSoundByName("play");
	PlaySoundByName("play", 7, -1);
}

void GSWord::Exit()
{
	StopSoundByName("play", 7);
}

void GSWord::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSWord::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSWord::Update(float deltaTime) {
	
}

void GSWord::Draw(){
	DrawVectorObject(m_objectVector);	
	
	for (auto& button : m_buttonList)
		button->Draw();

	DrawVectorObject(m_choice);	

	if (!GSMachine::GetInstance()->IsRunning())	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto &button : m_pauseButtonList)
			button->Draw();
	}	
}

void GSWord::HandleEvents()
{
}

void GSWord::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSWord::HandleTouchEvents(float x, float y, bool bIsPressed) {
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
					printf("Correct\n");
					key.erase(i, 1);
					isCorrect = true;
					UpdateChoiceObjects();
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

void GSWord::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSWord::UpdateChoiceObjects() {
	int totalWidth = key.size() * 50 + (key.size() - 1) * 30;
	int leftAlign = (1280 - totalWidth) / 2;  
	m_choice.clear();

	for (int i = 0; i < key.size(); i++) {
		std::string fileName = std::string(1, key[i]) + "02";
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(leftAlign + i * 80, 400);  
		slot->SetSize(50, 50);  
		m_choice.push_back(slot);  
	}
}