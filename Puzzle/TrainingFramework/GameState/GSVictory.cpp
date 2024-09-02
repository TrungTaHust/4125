#include "../TrainingFramework/stdafx.h"
#include "GSVictory.h"
#include "../TrainingFramework/stdafx.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "../TrainingFramework/GameState/GSMachine.h"
#include "../TrainingFramework/GameObject/Text.h"

GSVictory::GSVictory()
{
	m_score = 0;
	m_stateType = STATE_VICTORY;
}

GSVictory::~GSVictory()
{
}

void GSVictory::Init()
{
	m_overBackground = SceneManager::GetInstance()->GetObjectByID("over_background");
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_back_to_menu"));
	m_text.SetPos({ 640, 190, 1 }); // Đặt vị trí của văn bản
	m_text.GetTextColor() = SDL_Color({ 255, 255, 255, 255 }); // Đặt màu sắc của văn bản
	m_text.GetTextSize() = 80; // Đặt kích thước của văn bản
	m_text.GetTextMessage() = "VICTORY"; // Đặt nội dung văn bản
	m_text.Init("../Resources/Fonts/orbitron/Orbitron-Black.ttf");
	AddSoundByName("over");
	PlaySoundByName("over", 9, -1);
}

void GSVictory::Exit()
{
	StopSoundByName("over", 9);
}

void GSVictory::Pause()
{
	PauseSoundByName("over");
}

void GSVictory::Resume()
{
	ResumeSoundByName("over");
}

void GSVictory::Update(float deltaTime)
{
	
}

void GSVictory::Draw()
{
	m_overBackground->Draw();
	m_text.Draw();
	for (auto& button : m_buttonList)
		button->Draw();
}

void GSVictory::HandleEvents()
{
}

void GSVictory::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSVictory::HandleTouchEvents(float x, float y, bool bIsPressed)
{
	for (auto& button : m_buttonList) {
		if (button->HandleTouchEvent(x, y, bIsPressed))
		{
			switch (button->m_type)
			{
			case BUTTON_BACK_TO_MENU:
				GSMachine::GetInstance()->PopState();
				GSMachine::GetInstance()->PopState();
				break;
			}
		};
	}
}

void GSVictory::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
	{
		button->HandleMoveEvent(x, y);
	}
}