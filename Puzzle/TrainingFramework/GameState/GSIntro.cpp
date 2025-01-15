#include "../TrainingFramework/stdafx.h"
#include "GSIntro.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameManager/SceneManager.h"


GSIntro::GSIntro()
{
	m_stateType = STATE_INTRO;
	m_ftime = 1;
}

GSIntro::~GSIntro()
{
}

void GSIntro::Init()
{
	m_intro = SceneManager::GetInstance()->GetObjectByID("intro_logo");
	AddSoundByName("intro");
	PlaySoundByName("intro", 0, -1);
}

void GSIntro::Exit()
{
	StopSoundByName("intro", 0);
}

void GSIntro::Pause()
{
	StopSoundByName("intro", 0);
	//PauseSoundByName("intro");
}

void GSIntro::Resume()
{
	ResumeSoundByName("intro");
}

void GSIntro::Update(float deltaTime)
{
	m_ftime -= deltaTime;
	m_intro->SetAlpha(1 - m_ftime / 3);
	if (m_ftime < 0)
	{
		GSMachine::GetInstance()->ChangeState(StateType::STATE_MENU);
	}
}

void GSIntro::Draw()
{
	m_intro->Draw();
}

void GSIntro::HandleEvents()
{
}

void GSIntro::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSIntro::HandleTouchEvents(float x, float y, bool bIsPressed)
{
}

void GSIntro::HandleMouseMoveEvents(float x, float y)
{
}