﻿#include "../TrainingFramework/stdafx.h"
#include "GSSelectMap.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameObject/Text.h"

GSSelectMap::GSSelectMap()
{
	m_stateType = STATE_SELECT_MAP;
}

GSSelectMap::~GSSelectMap()
{
}

void GSSelectMap::Init()
{
	m_menuBackground = SceneManager::GetInstance()->GetObjectByID("menu_background");
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map1"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map2"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map3"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map4"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map5"));
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("map6"));
	m_text.SetPos({ 640, 150, 1 }); // Đặt vị trí của văn bản
	m_text.GetTextColor() = SDL_Color({ 255, 255, 255, 255 }); // Đặt màu sắc của văn bản
	m_text.GetTextSize() = 80; // Đặt kích thước của văn bản
	m_text.GetTextMessage() = "Select map to play"; // Đặt nội dung văn bản
	m_text.Init("../Resources/Fonts/orbitron/Orbitron-Black.ttf");
	AddSoundByName("menu");
	PlaySoundByName("menu", 1, -1);
}

void GSSelectMap::Exit()
{
	StopSoundByName("menu", 1);
}

void GSSelectMap::Pause()
{
	PauseSoundByName("menu");
}

void GSSelectMap::Resume()
{
	ResumeSoundByName("menu");
}

void GSSelectMap::Update(float deltaTime)
{
	
}

void GSSelectMap::Draw()
{
	m_menuBackground->Draw();
	m_text.Draw();
	for (auto& button : m_buttonList)
		button->Draw();
	for (auto& chara : m_someAnim)
		chara->Draw();
}

void GSSelectMap::HandleEvents()
{
}

void GSSelectMap::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSSelectMap::HandleTouchEvents(float x, float y, bool bIsPressed)
{
	for (auto& button : m_buttonList)
	{
		if (button->HandleTouchEvent(x, y, bIsPressed))
		{
			switch (button->m_type)
			{
			case BUTTON_MAP1:
				//currentMap = 1;
				GSMachine::GetInstance()->PushState(StateType::STATE_CAMPAIGN_1);
				break;			
			case BUTTON_EXIT:
				exit(0);
			}
		};
	}
}

void GSSelectMap::HandleMouseMoveEvents(float x, float y)
{	
}