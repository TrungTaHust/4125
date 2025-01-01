#include "../TrainingFramework/stdafx.h"
#include "GSAnim2.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSAnim2::GSAnim2() : index(0), isCorrect(false)
{
	m_stateType = STATE_ANIM2;		
}

GSAnim2::~GSAnim2()
{
}

void GSAnim2::Init()
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

	m_question = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
	m_question->Set2DPos(640, 200);
	m_question->SetSize(200, 200);

	NewQuestion();	
	UpdateChoiceObjects();	

	m_objectVector.push_back(m_question);
	AddSoundByName("play");

	AddSoundByName("correct");
	PlaySoundByName("play", 7, -1);
	m_time = 1;

}

void GSAnim2::Exit()
{
	StopSoundByName("play", 7);
}

void GSAnim2::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSAnim2::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSAnim2::Update(float deltaTime) {
	UpdateChoiceObjects();
	if (isCorrect) {
		m_time -= deltaTime;
		if (m_time <= 0) {
			isCorrect = false;
			m_time += 1;
			NewQuestion();			
		}
	}

}

void GSAnim2::Draw(){
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

void GSAnim2::HandleEvents()
{
}

void GSAnim2::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSAnim2::HandleTouchEvents(float x, float y, bool bIsPressed) {
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
					GSMachine::GetInstance()->Resume();
					GSMachine::GetInstance()->PopState();
					break;
				case BUTTON_TUTORIAL:
					GSMachine::GetInstance()->Resume();
					break;
				}
			};
	}
}

void GSAnim2::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSAnim2::UpdateChoiceObjects() {
	int totalWidth = key.size() * 100 + (key.size() - 1) * 20;
	int leftAlign = (1280 - totalWidth) / 2;
	m_choice.clear();
	for (int i = 0; i < key.size(); i++) {
		std::string fileName = std::string(1, key[i]);
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(leftAlign + i * 120, 400);
		slot->SetSize(150, 150);
		m_choice.push_back(slot);
	}
}

void GSAnim2::NewQuestion() {
	key = animals[rand() % animals.size()];
	m_question->SetTexture(key.c_str());	
	char c;
	do {
		c = rand() % 26 + 'a'; 
		index = rand() % (key.size() + 1); 
	} while ((index > 0 && c == key[index - 1]) || (index < key.size() && c == key[index])); 

	key.insert(index, 1, c); 
}