#include "../TrainingFramework/stdafx.h"
#include "GSAnim1.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSAnim1::GSAnim1() {
	m_stateType = STATE_ANIM1;
}

GSAnim1::~GSAnim1()
{
}

void GSAnim1::Init()
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

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {			
			auto slot = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
			slot->Set2DPos(400 + i * 240, 400 + j * 200);
			slot->SetSize(200, 200);
			m_choice.push_back(slot);
		}
	}

	m_question = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
	m_question->Set2DPos(640, 200);
	m_question->SetSize(300, 200);	

	m_objectVector.push_back(m_question);
	NewQuestion();

	AddSoundByName("play");
	PlaySoundByName("play", 7, -1);
	AddSoundByName("correct");

	AddText("your_scores");
	AddText("end_scores");
	m_scoreFrame = SceneManager::GetInstance()->GetObjectByID("score_frame");
}

void GSAnim1::Exit()
{
	StopSoundByName("play", 7);
}

void GSAnim1::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSAnim1::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSAnim1::Update(float deltaTime) {
	if (!isCompleted) {
		if (isCorrect) {
			m_time -= deltaTime;
			if (m_time <= 0) {
				isCorrect = false;
				m_time = 1;
				NewQuestion();
			}
		}
	}
	else {
		UpdateText("end_scores", anim1Value, deltaTime);
		end_time -= deltaTime;
		if (end_time <= 0)	GSMachine::GetInstance()->PushState(STATE_GAMEOVER);
	}
}

void GSAnim1::Draw(){
	DrawVectorObject(m_objectVector);	
	
	for (auto& button : m_buttonList)
		button->Draw();

	DrawVectorObject(m_choice);	

	if (!GSMachine::GetInstance()->IsRunning())	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto &button : m_pauseButtonList)
			button->Draw();
	}	

	if (isCompleted) {
		m_scoreFrame->Draw();
		RenderText("your_scores");
		RenderText("end_scores");
	}
}

void GSAnim1::HandleEvents()
{
}

void GSAnim1::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSAnim1::HandleTouchEvents(float x, float y, bool bIsPressed) {
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
		
	if(!isCorrect)
		for (auto& button : m_choice) {
			if (button->HandleTouchEvent(x, y, bIsPressed)) {
				std::string result = "q_" + button->getTexture()->GetID();
				totalClick++;
				if (result == m_question->getTexture()->GetID()) {
					printf("Correct\n");
					correctAns++;
					isCorrect = true;
					PlaySoundByName("correct", 8, 0);
				}
				if (totalClick == 20 || correctAns == 5) {
					anim1Value = correctAns * 100 / totalClick;
					updateData("anim1", anim1Value);	
					isCompleted = true;
				}
				printf("Total click: %d\nCorrect: %d\nScore: %d\n", totalClick, correctAns, correctAns * 100 / totalClick);
			}
		}
	}
	else {
		for (auto& button : m_pauseButtonList)
			if (button->HandleTouchEvent(x, y, bIsPressed))	{
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
					tuto_time = 2;
					break;
				}
			};
	}
}

void GSAnim1::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSAnim1::NewQuestion() {
	int key = rand() % animals.size();

	std::set<int> uniqueIndices;
	uniqueIndices.insert(key);
	while (uniqueIndices.size() < 6) {
		int randomIndex = rand() % animals.size();
		if (randomIndex != key) {
			uniqueIndices.insert(randomIndex);
		}
	}

	std::vector<int> indices(uniqueIndices.begin(), uniqueIndices.end());
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(indices.begin(), indices.end(), g);

	auto it = indices.begin();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			std::string fileName = animals[*it];
			m_choice[j * 3 + i]->SetTexture(fileName.c_str());
			++it;
		}
	}

	std::string fileName = "q_" + animals[key];
	m_question->SetTexture(fileName.c_str());
	return;
}