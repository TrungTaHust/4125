#include "../TrainingFramework/stdafx.h"
#include "../TrainingFramework/Globals.h"
#include "GS_Campaign_1.h"

GSCampaign1::GSCampaign1()
{
	m_stateType = STATE_CAMPAIGN_1;
}

GSCampaign1::~GSCampaign1()
{
}

void GSCampaign1::Init()
{
	auto background = SceneManager::GetInstance()->GetObjectByID("map1");
	background->Set2DPos(640, 480);
	background->SetSize(1280, 960);
	m_objectVector.push_back(background);
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("human_base"));
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("play_upper_pane"));
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("play_lower_pane"));

	AddAnimation("loading_animation");
	AddAnimation("coins");


	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_pause"));

	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_resume_2"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_home"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_tutorial"));

	AddText("reloading");
	AddText("current_bullets");
	AddText("scores");
	AddText("gsplay_scores");
	AddText("your_bullets");
	AddText("coins");

	AddSoundByName("play");
	AddSoundByName("explosion");
	AddSoundByName("reload");

	PlaySoundByName("play", 7, -1);
	//printf("%d", currentMap);
}

void GSCampaign1::Exit()
{
	StopSoundByName("explosion", 2);
	StopSoundByName("play", 7);
	StopSoundByName("reload", 8);
}

void GSCampaign1::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSCampaign1::Resume()
{
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSCampaign1::Update(float deltaTime)
{	
	
}

void GSCampaign1::Draw()
{
	DrawVectorObject(m_objectVector);

	for (auto& button : m_buttonList)
		button->Draw();


	if (!GSMachine::GetInstance()->IsRunning())
	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto button : m_pauseButtonList)
			button->Draw();
	}
}

void GSCampaign1::HandleEvents()
{
}

void GSCampaign1::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSCampaign1::HandleTouchEvents(float x, float y, bool bIsPressed)
{
	for (auto& button : m_buttonList) {
		if (button->HandleTouchEvent(x, y, bIsPressed))
		{
			switch (button->m_type)
			{
			case BUTTON_PAUSE:
				GSMachine::GetInstance()->Pause();
				button->SetAlpha(0.5f);
				break;
			}
		};
	}

	if (!GSMachine::GetInstance()->IsRunning())
	{
		for (auto& button : m_pauseButtonList) {
			if (button->HandleTouchEvent(x, y, bIsPressed))
			{
				GSMachine::GetInstance()->Resume();
				switch (button->m_type)
				{
				case BUTTON_RESUME:
					break;
				case BUTTON_BACK_TO_MENU:
					GSMachine::GetInstance()->PopState();
					break;
				case BUTTON_TUTORIAL:
					GSMachine::GetInstance()->PushState(STATE_TUTORIAL);
					break;
				}
			};
		}
	}
}

void GSCampaign1::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
	{
		button->HandleMoveEvent(x, y);
	}

	if (!GSMachine::GetInstance()->IsRunning())
	{
		for (auto& button : m_pauseButtonList)
		{
			button->HandleMoveEvent(x, y);
		}
	}
}