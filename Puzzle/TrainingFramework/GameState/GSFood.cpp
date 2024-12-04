#include "../TrainingFramework/stdafx.h"
#include "GSFood.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSFood::GSFood() : index(0), isCorrect(false)
{
	m_stateType = STATE_PUZZLE;		
	m_time = 1;
}

GSFood::~GSFood()
{
}

void GSFood::Init()
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

	m_basket = std::make_shared<Object>("Sprite2D", "basket", "TriangleShader");
	m_basket->SetSize(200, 200);

	m_question = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
	m_question->Set2DPos(1040, 200);
	m_question->SetSize(200, 200);
	NewQuestion();
	m_objectVector.push_back(m_question);

	AddSoundByName("play");
	AddSoundByName("correct");
	PlaySoundByName("play", 7, -1);		
}

void GSFood::Exit()
{
	StopSoundByName("play", 7);
}

void GSFood::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSFood::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSFood::Update(float deltaTime) {
	m_time -= deltaTime;
	if (m_time <= 0) {
		m_time += 1.5;
		NewFruit();
	}
	std::vector<std::shared_ptr<Object>> falling;

	for (auto &fruit : m_falling) {
		fruit->Set2DPos(fruit->GetPos().x, fruit->GetPos().y + deltaTime * 200);
		if (fruit->GetPos().y <= 900 && !fruit->CheckCollide(m_basket))
			falling.push_back(fruit);

		if (fruit->CheckCollide(m_basket))
			if (fruit->getTexture() == m_question->getTexture()) {
				PlaySoundByName("correct", 8, 0);
				NewQuestion();
			}
	}
	m_falling = falling;
}

void GSFood::Draw(){
	DrawVectorObject(m_objectVector);	
	
	for (auto& button : m_buttonList)
		button->Draw();

	if (!GSMachine::GetInstance()->IsRunning())	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto &button : m_pauseButtonList)
			button->Draw();
	}	
	m_basket->Draw();
	DrawVectorObject(m_falling);
}

void GSFood::HandleEvents()
{
}

void GSFood::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSFood::HandleTouchEvents(float x, float y, bool bIsPressed) {
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

void GSFood::HandleMouseMoveEvents(float x, float y)
{
	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
	else {
		for (auto& button : m_buttonList)
			button->HandleMoveEvent(x, y);
		m_basket->Set2DPos(x, 800);
	}
}

void GSFood::NewFruit() {
	if (m_falling.size() >= 7) return;
	key = fruits[rand() % fruits.size()];
	auto fruit = std::make_shared<Object>("Sprite2D", key.c_str(), "TriangleShader");
	int x = (rand() % 5) * 150 + 340;
	fruit->Set2DPos(x, 50);
	fruit->SetSize(100, 100);
	m_falling.push_back(fruit);
}

void GSFood::NewQuestion() {
	index = rand() % fruits.size();
	m_question->SetTexture(fruits[index].c_str());
}
