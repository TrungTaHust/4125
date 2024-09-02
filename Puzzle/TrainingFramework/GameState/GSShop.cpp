#include "../TrainingFramework/stdafx.h"
#include "GSShop.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameObject/Text.h"
//#include "../Resources/Fonts/orbitron/"

GSShop::GSShop()
{
	m_stateType = STATE_SHOP;
}

GSShop::~GSShop()
{
}

void GSShop::Init()
{
	m_menuBackground = SceneManager::GetInstance()->GetObjectByID("menu_background");
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map1"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map2"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map3"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map4"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map5"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map6"));
	m_text.SetPos({ 640, 190, 1 }); // Đặt vị trí của văn bản
	m_text.GetTextColor() = SDL_Color({ 255, 255, 255, 255 }); // Đặt màu sắc của văn bản
	m_text.GetTextSize() = 80; // Đặt kích thước của văn bản
	m_text.GetTextMessage() = "Stop the Aliens"; // Đặt nội dung văn bản
	m_text.Init("../Resources/Fonts/orbitron/Orbitron-Black.ttf");
	AddSoundByName("menu");
	PlaySoundByName("menu", 1, -1);
}

void GSShop::Exit()
{
	StopSoundByName("menu", 1);
}

void GSShop::Pause()
{
	PauseSoundByName("menu");
}

void GSShop::Resume()
{
	ResumeSoundByName("menu");
}

void GSShop::Update(float deltaTime)
{
	for (auto& chara : m_someAnim)
	{
		chara->Update(deltaTime);
	}
}

void GSShop::Draw()
{
	m_menuBackground->Draw();
	m_text.Draw();
	for (auto& button : m_buttonList)
	{
		button->Draw();
	}
	for (auto& chara : m_someAnim)
	{
		chara->Draw();
	}
}

void GSShop::HandleEvents()
{
}

void GSShop::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSShop::HandleTouchEvents(float x, float y, bool bIsPressed)
{
	for (auto& button : m_buttonList)
	{
		if (button->HandleTouchEvent(x, y, bIsPressed))
		{
			switch (button->m_type)
			{
			case BUTTON_MAP1:
				GSMachine::GetInstance()->PushState(StateType::STATE_CAMPAIGN_1);
				break;
			case BUTTON_MAP2:
				GSMachine::GetInstance()->PushState(StateType::STATE_CAMPAIGN_2);
				break;
			case BUTTON_MAP3:
				GSMachine::GetInstance()->PushState(StateType::STATE_CAMPAIGN_3);
				break;
			case BUTTON_MAP4:
				GSMachine::GetInstance()->PushState(StateType::STATE_CAMPAIGN_4);
				break;
			case BUTTON_MAP5:
				GSMachine::GetInstance()->PushState(StateType::STATE_CAMPAIGN_5);
				break;
			case BUTTON_MAP6:
				GSMachine::GetInstance()->PushState(StateType::STATE_CAMPAIGN_6);
				break;
			case BUTTON_EXIT:
				exit(0);
			}
		};
	}
}

void GSShop::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
	{
		button->HandleMoveEvent(x, y);
	}
}