#include "../TrainingFramework/stdafx.h"
#include "GSMenu.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameObject/Text.h"

GSMenu::GSMenu()
{
	m_stateType = STATE_MENU;
}

GSMenu::~GSMenu()
{
}

void GSMenu::Init()
{
	m_menuBackground = SceneManager::GetInstance()->GetObjectByID("menu_background");

	auto sceneManager = SceneManager::GetInstance();
	
	m_buttonList = {
		sceneManager->GetButtonByID("button_animals1"),
		sceneManager->GetButtonByID("button_animals2"),
		sceneManager->GetButtonByID("button_action"),
		sceneManager->GetButtonByID("button_food"),
		sceneManager->GetButtonByID("button_vehicles"),
		sceneManager->GetButtonByID("button_number"),
		sceneManager->GetButtonByID("button_exit")
	};

	m_text.SetPos({ 640, 190, 1 }); // Đặt vị trí của văn bản
	m_text.GetTextColor() = SDL_Color({ 255, 255, 255, 255 }); // Đặt màu sắc của văn bản
	m_text.GetTextSize() = 80; // Đặt kích thước của văn bản
	m_text.GetTextMessage() = "WordHunter"; // Đặt nội dung văn bản
	m_text.Init("../Resources/Fonts/orbitron/Orbitron-Black.ttf");
	AddSoundByName("menu");
	PlaySoundByName("menu", 1, -1);
}

void GSMenu::Exit()
{
	StopSoundByName("menu", 1);
}

void GSMenu::Pause()
{
	PauseSoundByName("menu");
}

void GSMenu::Resume()
{
	ResumeSoundByName("menu");
}

void GSMenu::Update(float deltaTime)
{
	for (auto& chara : m_someAnim)
	{
		chara->Update(deltaTime);
	}

}

void GSMenu::Draw()
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

void GSMenu::HandleEvents()
{
}

void GSMenu::HandleKeyEvents(int key, bool bIsPressed)
{	
}

void GSMenu::HandleTouchEvents(float x, float y, bool bIsPressed)
{
	for (auto& button : m_buttonList)
	{
		if (button->HandleTouchEvent(x, y, bIsPressed))
		{
			switch (button->m_type)
			{
			case BUTTON_ANIMALS1:
				GSMachine::GetInstance()->PushState(StateType::STATE_ANIM1);
				break;
			case BUTTON_ANIMALS2:
				GSMachine::GetInstance()->PushState(StateType::STATE_ANIM2);
				break;
			case BUTTON_ACTION:
				GSMachine::GetInstance()->PushState(StateType::STATE_ACTION);
				break;
			case BUTTON_FOOD:
				GSMachine::GetInstance()->PushState(StateType::STATE_FOOD);
				break;			
			case BUTTON_NUMBER:
				GSMachine::GetInstance()->PushState(StateType::STATE_COUNT);
				break;
			case BUTTON_VEHICLES:
				GSMachine::GetInstance()->PushState(StateType::STATE_COUNT2);
				break;
			case BUTTON_EXIT:
				exit(0);
			}
		};
	}
}

void GSMenu::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
	{
		button->HandleMoveEvent(x, y);
	}
}