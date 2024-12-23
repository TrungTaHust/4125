#include "../TrainingFramework/stdafx.h"
#include "GSFood.h"
#include <set>
#include <algorithm>
#include <random>
#include <iterator>

GSFood::GSFood() : index(0), isCorrect(false)
{
	m_stateType = STATE_FOOD;		
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
		m_time += 1;
		NewFruit();
	}
	std::vector<std::shared_ptr<Object>> falling;

	for (auto &letter : m_falling) {
		letter->Set2DPos(letter->GetPos().x, letter->GetPos().y + deltaTime * 200);
		if (letter->GetPos().y <= 900 && !letter->CheckCollide(m_basket))
			falling.push_back(letter);

		if (letter->CheckCollide(m_basket)) {
			std::string key = m_question->getTexture()->GetID();
			std::string drop = letter->getTexture()->GetID();

			for (int i = 0; i < key.size(); i++ )
				if (drop[0] == key[i] && m_textAnswer[i]->getTexture()->GetID() == "star") {
					PlaySoundByName("correct", 8, 0);
					m_textAnswer[i]->SetTexture(drop.c_str());

					auto it = std::find(nameChars.begin(), nameChars.end(), drop[0]);
					if (it != nameChars.end()) {
						nameChars.erase(it);
						if (std::find(nameChars.begin(), nameChars.end(), drop[0]) == nameChars.end())
							nonNameChars.push_back(drop[0]);
					}
					break;
				}
			if (nameChars.size() == 0) NewQuestion();
		}
	}
	m_falling = falling;
}

void GSFood::Draw(){
	DrawVectorObject(m_objectVector);	
	DrawVectorObject(m_textAnswer);

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
	if (m_falling.size() >= 7 || nameChars.size() == 0) return;
	int rate = rand() % 100;
	char c;
	int temp1 = nameChars.size();
	int temp2 = nonNameChars.size();

	if (rate < 70 && temp1 > 0)
		c = nameChars[rand() % temp1];
	else if (temp2 > 0)
		c = nonNameChars[rand() % temp2];
	else return;
	
	std::string fileName(1, c);
	auto fruit = std::make_shared<Object>("Sprite2D", fileName.c_str(), "TriangleShader");
	int x = (rand() % 5) * 150 + 340;
	fruit->Set2DPos(x, 50);
	fruit->SetSize(100, 100);
	m_falling.push_back(fruit);
}

void GSFood::NewQuestion() {
	index = rand() % fruits.size();
	//key = fruits[index];
	key = "apple";
	m_question->SetTexture(key.c_str());

	nonNameChars.clear();
	nameChars.clear();

	std::vector<int> charCount(26, 0);

	for (char c : key) {
		if (std::isalpha(c)) {  
			charCount[c - 'a']++;
		}
	}

	for (char c = 'a'; c <= 'z'; ++c) {
		if (charCount[c - 'a'] > 0) 
			nameChars.insert(nameChars.end(), charCount[c - 'a'], c);
		else 
			nonNameChars.push_back(c);		
	}

	int totalWidth = key.size() * 100 + (key.size() - 1) * 20;
	int leftAlign = (1280 - totalWidth) / 2;
	m_textAnswer.clear();
	for (int i = 0; i < key.size(); i++) {
		auto slot = std::make_shared<Object>("Sprite2D", "star", "TriangleShader");
		slot->Set2DPos(leftAlign + i * 120, 200);
		slot->SetSize(100, 100);
		m_textAnswer.push_back(slot);
	}
}
