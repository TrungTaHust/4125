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

	for (int i = 0; i < 3; i++) {
		auto slot = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
		slot->Set2DPos(440 + i * 200, 600);
		slot->SetSize(150, 150);
		m_choice.push_back(slot);
	}

	m_question = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
	m_question->Set2DPos(640, 300);
	m_question->SetSize(700, 400);

	NewQuestion();	

	m_objectVector.push_back(m_question);
	AddSoundByName("play");

	AddSoundByName("correct");
	AddSoundByName("error");
	PlaySoundByName("play", 7, -1);	

	m_tuto = std::make_shared<Object>("Sprite2D", "tuto_count2", "TriangleShader");
	m_tuto->Set2DPos(640, 480);
	m_tuto->SetSize(800, 400);

	m_cursor = std::make_shared<Object>("Sprite2D", "hand", "TriangleShader");
	m_cursor->Set2DPos(640, 480);
	m_cursor->SetSize(50, 50);
	m_objectVector.push_back(m_cursor);

	AddText("end_scores");
	m_scoreFrame = SceneManager::GetInstance()->GetObjectByID("score_frame");
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
	if(!isCompleted) {
		if (isCorrect) {
			m_time -= deltaTime;
			if (m_time <= 0) {
				isCorrect = false;
				m_time = 1;
				NewQuestion();
			}
		}
		if (m_tutoTime >= 0) m_tutoTime -= deltaTime;
	}

	else {
		value = count * 100 / click;
		UpdateText("end_scores", value, deltaTime);
		end_time -= deltaTime;
		if (end_time <= 0)
			GSMachine::GetInstance()->PushState(STATE_GAMEOVER);
	}
	if (click == 20 || count == 5) isCompleted = true;

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

	if (m_tutoTime >= 0) m_tuto->Draw();

	if (isCompleted) {
		m_scoreFrame->Draw();
		RenderText("end_scores");
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
			{
				click++;
				if (m_choice[i]->getTexture()->GetID()[1] == m_question->getTexture()->GetID()[0]) {
					isCorrect = true;
					count++;
					PlaySoundByName("correct", 8, 0);
				}
				else PlaySoundByName("error", 9, 0);
			}
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
					m_tutoTime = 2;

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

	m_cursor->Set2DPos(x, y);
}

void GSCount::NewQuestion() {
	key = rand() % 10;
	m_question->SetTexture((std::to_string(key) + "_cow1").c_str());		

	std::set<int> uniqueIndices;
	uniqueIndices.insert(key);
	while (uniqueIndices.size() < 3) {
		int randomIndex = rand() % 10;
		if (randomIndex != key) {
			uniqueIndices.insert(randomIndex);
		}
	}

	std::vector<int> indices(uniqueIndices.begin(), uniqueIndices.end());
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(indices.begin(), indices.end(), g);
	
	for (int i = 0; i < 3; i++) {
		std::string fileName = "0" + std::to_string(indices[i]);
		m_choice[i]->SetTexture(fileName.c_str());		
	}	
}