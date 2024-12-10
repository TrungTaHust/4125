#include "../TrainingFramework/stdafx.h"
#include "GSCount.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSCount::GSCount() : isCorrect(false)
{
	m_stateType = STATE_COUNT;		
}

GSCount::~GSCount()
{
}

void GSCount::Init()
{
	srand(static_cast<unsigned>(time(0)));
	count = 0;

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

	m_objectVector.push_back(m_question);
	AddSoundByName("play");

	AddSoundByName("correct");
	PlaySoundByName("play", 7, -1);
}

void GSCount::Exit()
{
	StopSoundByName("play", 7);
}

void GSCount::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSCount::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSCount::Update(float deltaTime) {
	if (isCorrect) {
		m_time -= deltaTime;
		if (m_time <= 0) {
			isCorrect = false;
			m_time = 1;
			NewQuestion();
		}
	}
	else
		for (auto& it : m_choice)
			if (it->GetTouch())	{
				it->Set2DPos(it->GetPos().x + 400 * deltaTime, it->GetPos().y);
				if (it->GetPos().x >= 1000) {
					std::string fileName = color[rand() % color.size()];

					PlaySoundByName("correct", 8, 0);
					it->Set2DPos(300, it->GetPos().y);
					it->SetTexture(fileName.c_str());
					it->SetTouch(false);

					count++;
					if (key == count) {
						PlaySoundByName("correct", 8, 0);
						printf("Correct\n");
						count = 0;
						isCorrect = true;
						return;
					}
				}
			}

}

void GSCount::Draw(){
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

void GSCount::HandleEvents()
{
}

void GSCount::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSCount::HandleTouchEvents(float x, float y, bool bIsPressed) {
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
		
		for (int i = 0; i < m_choice.size(); i++) 
			if (m_choice[i]->HandleTouchEvent(x, y, bIsPressed)) 
				m_choice[i]->SetTouch(true);		
	}
	else {
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
					GSMachine::GetInstance()->PushState(STATE_TUTORIAL);
					break;
				}
			};
	}
}

void GSCount::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSCount::NewQuestion() {
	key = rand() % 8 + 1;
	m_question->SetTexture(("0" + std::to_string(key)).c_str());
	m_choice.clear();
	for (int i = 0; i < 5; i++) {
		std::string fileName = color[rand() % color.size()];
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(300, 240 + i * 120);
		slot->SetSize(100, 100);
		m_choice.push_back(slot);
	}
}