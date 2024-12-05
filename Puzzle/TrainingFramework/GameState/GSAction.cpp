#include "../TrainingFramework/stdafx.h"
#include "GSAction.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSAction::GSAction() : index(0), isCorrect(false)
{
	m_stateType = STATE_ACTION;		
}

GSAction::~GSAction()
{
}

void GSAction::Init()
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

	for (int i = 0; i < 6; i++) {		
		auto slot = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
		slot->Set2DPos(290 + i * 120, 700);
		slot->SetSize(100, 100);
		m_keyboard.push_back(slot);
	}

	m_question = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
	m_question->Set2DPos(640, 200);
	m_question->SetSize(200, 200);

	NewQuestion();	

	m_objectVector.push_back(m_question);
	AddSoundByName("play");

	AddSoundByName("correct");
	PlaySoundByName("play", 7, -1);	
}

void GSAction::Exit()
{
	StopSoundByName("play", 7);
}

void GSAction::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSAction::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSAction::Update(float deltaTime) {
	if (isCorrect) {
		UpdateChoiceObjects();
		m_time -= deltaTime;
		if (m_time <= 0) {
			isCorrect = false;
			m_time = 1;
			NewQuestion();			
		}
	}
}

void GSAction::Draw(){
	DrawVectorObject(m_objectVector);	
	
	for (auto& button : m_buttonList)
		button->Draw();

	DrawVectorObject(m_choice);
	DrawVectorObject(m_keyboard);

	if (!GSMachine::GetInstance()->IsRunning())	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto &button : m_pauseButtonList)
			button->Draw();
	}	
}

void GSAction::HandleEvents()
{
}

void GSAction::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSAction::HandleTouchEvents(float x, float y, bool bIsPressed) {
	if (GSMachine::GetInstance()->IsRunning()) {
		for (auto& button : m_buttonList) 
			if (button->HandleTouchEvent(x, y, bIsPressed)) {
				switch (button->m_type) {
				case BUTTON_PAUSE:
					GSMachine::GetInstance()->Pause();
					button->SetAlpha(0.5f);
					break;
				};
			}			
		std::string str(1, c);
		for (auto& button : m_keyboard)
			if (button->HandleTouchEvent(x, y, bIsPressed)) {
				if (button->getTexture()->GetID() == str) {
					PlaySoundByName("correct", 8, 0);
					isCorrect = true;
				};
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

void GSAction::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_choice)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSAction::UpdateChoiceObjects() {
	int totalWidth = key.size() * 100 + (key.size() - 1) * 20;
	int leftAlign = (1280 - totalWidth) / 2;
	m_choice.clear();
	for (int i = 0; i < key.size(); i++) {
		std::string fileName = std::string(1, key[i]);
		auto slot = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
		slot->Set2DPos(leftAlign + i * 120, 400);
		slot->SetSize(100, 100);
		m_choice.push_back(slot);
	}
}

void GSAction::NewQuestion() {
	key = action[rand() % action.size()];
	m_question->SetTexture(key.c_str());
	int index = rand() % key.size();
	c = key[index];
	std::string str(1, c);
	UpdateChoiceObjects();
	m_choice[index]->SetTexture("star");

	std::set<char> uniqueChars;
	uniqueChars.insert(c);
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 25);
	while (uniqueChars.size() < 6) {
		char randomChar = static_cast<char>('a' + std::rand() % 26);
		if (uniqueChars.find(randomChar) == uniqueChars.end()) {
			uniqueChars.insert(randomChar);
		}
	}

	std::vector<char> shuffledChars(uniqueChars.begin(), uniqueChars.end());
	std::shuffle(shuffledChars.begin(), shuffledChars.end(), gen);

	for (int i = 0; i < 6; i++) {
		std::string fileName = std::string(1, shuffledChars[i]);
		m_keyboard[i]->SetTexture(fileName.c_str());
	}
}