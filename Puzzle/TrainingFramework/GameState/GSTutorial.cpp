#include "../TrainingFramework/stdafx.h"
#include "GSTutorial.h"
#include "../TrainingFramework/stdafx.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "../TrainingFramework/GameState/GSMachine.h"
#include "../TrainingFramework/GameObject/Text.h"

GSTutorial::GSTutorial() {
	m_stateType = STATE_TUTORIAL;
}

GSTutorial::~GSTutorial() {
}

void GSTutorial::Init() {
	m_tutorialBackground = SceneManager::GetInstance()->GetObjectByID("tutorial_background");
	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_back"));

	AddSoundByName("over");
	PlaySoundByName("over", 10, -1);
	
	AddText("tutorial1");
	AddText("tutorial2");
	AddText("tutorial3");
	AddText("tutorial4");
}

void GSTutorial::Exit() {
	StopSoundByName("over", 10);
}

void GSTutorial::Pause() {	
}

void GSTutorial::Resume() {
}

void GSTutorial::Update(float deltaTime) {
	UpdateText("tutorial1", "Click on the aliens to destroy them before they reach the wall", deltaTime);
	UpdateText("tutorial2", "Bigger aliens require more clicks", deltaTime);
	UpdateText("tutorial3", "Bigger aliens also drop more coins and scores", deltaTime);
	UpdateText("tutorial4", "Remember to use power up (Press 1, 2, 3, 4)", deltaTime);
	for (auto& animation : m_tutorialAnimation)
		animation->Update(deltaTime);
}

void GSTutorial::Draw() {
	m_tutorialBackground->Draw();
	for (auto& button : m_buttonList)
		button->Draw();
	for (auto& object : m_objectVector)
		object->Draw();
	RenderText("tutorial1");
	RenderText("tutorial2");
	RenderText("tutorial3");
	RenderText("tutorial4");
}


void GSTutorial::HandleEvents() {
}

void GSTutorial::HandleKeyEvents(int key, bool bIsPressed) {
}

void GSTutorial::HandleTouchEvents(float x, float y, bool bIsPressed) {
	for (auto& button : m_buttonList) {
		if (button->HandleTouchEvent(x, y, bIsPressed))
		{
			switch (button->m_type)
			{
			case BUTTON_BACK:
				GSMachine::GetInstance()->PopState();
				break;
			}
		};
	}
}

void GSTutorial::HandleMouseMoveEvents(float x, float y) {
	for (auto& button : m_buttonList)
	{
		button->HandleMoveEvent(x, y);
	}
}