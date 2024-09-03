#include "../TrainingFramework/stdafx.h"
#include "GSBase.h"
#include "GSIntro.h"
#include "GSMenu.h"
#include "GSOver.h"
#include "GSScore.h"
#include "GSTutorial.h"
#include "GS_Campaign_1.h"
#include "GSVictory.h"
#include "GSSelectMap.h"

GSBase::GSBase()
{
	m_stateType = STATE_INVALID;
}

GSBase::GSBase(StateType stateType) : m_stateType(stateType)
{
}

GSBase::~GSBase()
{
}

std::shared_ptr<GSBase> GSBase::CreateState(StateType stateType)
{
	std::shared_ptr<GSBase> state;
	switch (stateType)
	{
	case STATE_INVALID:
		break;
	case STATE_INTRO:
		return std::make_shared<GSIntro>();
	case STATE_MENU:
		return std::make_shared<GSMenu>();
	case STATE_GAMEOVER:
		return std::make_shared<GSOver>();
	case STATE_SCORE:
		return std::make_shared<GSScore>();
	case STATE_TUTORIAL:
		return std::make_shared<GSTutorial>();
	case STATE_CAMPAIGN_1:
		return std::make_shared<GSCampaign1>();	
	case STATE_SELECT_MAP:
		return std::make_shared<GSSelectMap>();
	case STATE_VICTORY:
		return std::make_shared<GSVictory>();
	default:
		break;
	}
	return nullptr;
}

StateType GSBase::GetGameStateType()
{
	return m_stateType;
}

void GSBase::AddSoundByName(const char* soundName)
{
	auto sound = ResourceManager::GetInstance()->GetSoundPointerByName(soundName);
	auto name = std::string(soundName);
	m_soundMap.insert(std::make_pair(name, sound));
}

void GSBase::PlaySoundByName(const char* soundName, int channel, int loop)
{
	auto it = m_soundMap.find(soundName);
	if (it != m_soundMap.end()) {
		it->second.get()->Play(channel, loop);
	}
}

void GSBase::PauseSoundByName(const char* soundName)
{
	auto it = m_soundMap.find(soundName);
	if (it != m_soundMap.end()) {
		it->second.get()->Pause();
	}
}

void GSBase::ResumeSoundByName(const char* soundName)
{
	auto it = m_soundMap.find(soundName);
	if (it != m_soundMap.end()) {
		it->second.get()->Resume();
	}
}

void GSBase::StopSoundByName(const char* soundName, int channel)
{
	auto it = m_soundMap.find(soundName);
	if (it != m_soundMap.end()) {
		it->second.get()->Stop(channel);
	}
}

void GSBase::AddAnimation(const char* name)
{
	std::string Name = std::string(name);
	m_animationMap.insert(std::make_pair(Name, SceneManager::GetInstance()->GetAnimationByID(name)));
}

void GSBase::DrawAnimation(const char* name)
{
	auto it = m_animationMap.find(name);
	if (it != m_animationMap.end())
	{
		auto& animPointer = it->second;
		animPointer->Draw();
	}
}

void GSBase::AddText(const char* name)
{
	std::string Name = std::string(name);
	m_textMap.insert(std::make_pair(Name, ResourceManager::GetInstance()->GetTextPointerByName(name)));
}

void GSBase::RenderText(const char* name)
{
	auto it = m_textMap.find(name);
	if (it != m_textMap.end())
	{
		auto& textPointer = it->second;
		textPointer->Draw();
	}
}

void GSBase::RenderTextVector(std::vector<std::shared_ptr<Text>> textVector)
{
	for (auto textPointer : textVector)
		textPointer->Draw();
}

void GSBase::UpdateText(const char* name, const char* message, float deltaTime)
{
	auto it = m_textMap.find(name);
	if (it != m_textMap.end())
	{
		it->second->GetTextMessage() = std::string(message);
		it->second->Update(deltaTime);
	}
}

void GSBase::UpdateText(const char* name, float message, float deltaTime)
{
	int intValue = static_cast<int>(message);
	std::string convertedMessage = std::to_string(intValue);

	auto it = m_textMap.find(name);
	if (it != m_textMap.end())
	{
		it->second->GetTextMessage() = convertedMessage;
		it->second->Update(deltaTime);
	}
}

void GSBase::DrawVectorObject(std::vector<std::shared_ptr<Object>> objectVector)
{
	for (auto& it : objectVector)
		it->Draw();
}
