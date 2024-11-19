#include "../TrainingFramework/stdafx.h"
#include "GSSelectMap.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameObject/Text.h"

std::string Globals::map;

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
	std::ifstream inputFile("../TrainingFramework/map.txt");
	if (inputFile.is_open()) {
		Globals::map.clear();
		getline(inputFile, Globals::map);
		inputFile.close();
	}
	
	for (int i = 0; i < strlen(Globals::map.c_str()); i++)
		if (Globals::map[i] == '0')
			m_buttonList[i]->SetAlpha(0.4);
		/*else
			m_buttonList[i]->SetAlpha(1);*/
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
			
		};
	}
}

void GSSelectMap::HandleMouseMoveEvents(float x, float y)
{
	for (int i = 0; i < strlen(Globals::map.c_str()); i++)
		if (Globals::map[i] == '1')
			m_buttonList[i]->HandleMoveEvent(x, y);		
}