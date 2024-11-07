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
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_start"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_endless"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_exit"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("long_button_tutorial"));
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
			case BUTTON_START:
				GSMachine::GetInstance()->PushState(StateType::STATE_PUZZLE);
				break;
			case BUTTON_ENDLESS:
				GSMachine::GetInstance()->PushState(StateType::STATE_WORD);
				break;
			case BUTTON_TUTORIAL:
				GSMachine::GetInstance()->PushState(StateType::STATE_TUTORIAL);
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