#include "../TrainingFramework/stdafx.h"
#include "GS_Puzzle.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSPuzzle::GSPuzzle()
	: answer("     "), check("00000")
{
	m_stateType = STATE_PUZZLE;	
}

GSPuzzle::~GSPuzzle()
{
}

void GSPuzzle::Init()
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
			auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
			slot->Set2DPos(400 + i * 240, 400 + j * 200);
			slot->SetSize(200, 200);
			m_choice.push_back(slot);
			++it;
		}
	}	
	m_question = std::make_shared<Object>("Sprite2D", animals[key].c_str(), "TriangleShader");
	m_question->Set2DPos(640, 200);
	m_question->SetSize(200, 200);

	m_objectVector.push_back(m_question);
	AddSoundByName("play");
	PlaySoundByName("play", 7, -1);
	AddSoundByName("correct");
}

void GSPuzzle::Exit()
{
	StopSoundByName("play", 7);
}

void GSPuzzle::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSPuzzle::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSPuzzle::Update(float deltaTime) {
	if (isCorrect) {
		m_time -= deltaTime;
		if (m_time <= 0) {
			isCorrect = false;
			m_time = 2;

			int key = rand() % animals.size();

			m_question->SetTexture(animals[key].c_str()); // Change question

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
			m_choice.clear();

			auto it = indices.begin();
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					std::string fileName = animals[*it];
					auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
					slot->Set2DPos(400 + i * 240, 400 + j * 200);
					slot->SetSize(200, 200);
					m_choice.push_back(slot);
					++it;
				}
			}
		}
	}
}

void GSPuzzle::Draw(){
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

void GSPuzzle::HandleEvents()
{
}

void GSPuzzle::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSPuzzle::HandleTouchEvents(float x, float y, bool bIsPressed) {
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
		
		for (auto& button : m_choice) {
			if (button->HandleTouchEvent(x, y, bIsPressed)) {
				if (button->getTexture() == m_question->getTexture()) {
					printf("Correct\n");
					isCorrect = true;
					PlaySoundByName("correct", 8, 0);
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

void GSPuzzle::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}