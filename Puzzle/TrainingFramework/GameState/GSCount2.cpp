#include "../TrainingFramework/stdafx.h"
#include "GSCount2.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSCount2::GSCount2() : index(0), isCorrect(false)
{
	m_stateType = STATE_COUNT2;		
}

GSCount2::~GSCount2()
{
}

void GSCount2::Init()
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

	NewQuestion();
	
	AddSoundByName("play");
	AddSoundByName("correct");
	PlaySoundByName("play", 7, -1);
}

void GSCount2::Exit()
{
	StopSoundByName("play", 7);
}

void GSCount2::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSCount2::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSCount2::Update(float deltaTime) {
	if (!m_choice.empty()) {
		std::vector<std::shared_ptr<Object>> choice;
		for (auto& it : m_choice) {
			bool check = true;
			if (!it->GetTouch()) {
				std::vector<std::shared_ptr<Object>> questions;
				for (auto& question : m_question)
					if (it->CheckCollide(question) && it->getTexture()->GetID()[0] == question->getTexture()->GetID()[1]) {
						check = false;
						PlaySoundByName("correct", 8, 0);
					}
					else questions.push_back(question);
				m_question = questions;
			}
			if (check)
				choice.push_back(it);
		}

		m_choice = choice;
	}
	else NewQuestion();
}

void GSCount2::Draw(){
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

void GSCount2::HandleEvents()
{
}

void GSCount2::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSCount2::HandleTouchEvents(float x, float y, bool bIsPressed) {
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
			m_choice[i]->SetTouch(m_choice[i]->HandleTouchEvent(x, y, bIsPressed));			
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

void GSCount2::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice) {
		button->HandleMoveEvent(x, y);
		if (button->GetTouch())
			button->Set2DPos(x, y);
	}

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSCount2::UpdateChoiceObjects() {
	
}

void GSCount2::NewQuestion() {
	std::vector<int> uniqueIndices;
	while (uniqueIndices.size() < 3) {
		int randomIndex = rand() % color.size();
		if (std::find(uniqueIndices.begin(), uniqueIndices.end(), randomIndex) == uniqueIndices.end()) {
			uniqueIndices.push_back(randomIndex);
		}
	}
	m_choice.clear();
	m_question.clear();

	for (int i = 0; i < 3; i++) {
		std::string fileName = color[uniqueIndices[i]];
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(300 + i * 340, 300);
		slot->SetSize(300, 300);
		slot->SetTouch(false);
		m_choice.push_back(slot);
	}

	for (int i = 0; i < 3; i++) {
		std::string fileName = "0" + std::to_string(uniqueIndices[i] + 1);
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(300 + i * 340, 700);
		slot->SetSize(300, 300);
		m_question.push_back(slot);
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(m_question.begin(), m_question.end(), g);

	for (size_t i = 0; i < m_question.size(); i++)
		m_question[i]->Set2DPos(300 + i * 340, 700);
}