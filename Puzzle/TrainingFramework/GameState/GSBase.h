#pragma once
#include "../TrainingFramework/stdafx.h"
#include "../GameObject/Sound.h"
#include "../GameObject/Animation.h"
#include "../GameObject/Text.h"
#include <memory>
#include <map>
#include <string>
enum StateType {
	STATE_INVALID = 0,
	STATE_INTRO,
	STATE_MENU,
	STATE_PLAY,
	STATE_GAMEOVER,
	STATE_SCORE,
	STATE_TUTORIAL,
	STATE_SELECT_MAP,
	STATE_CAMPAIGN_1,
	STATE_VICTORY
};

class GSBase
{
public:
	GSBase();
	GSBase(StateType stateType);
	virtual ~GSBase();
	
	virtual void Init() = 0;
	virtual void Exit() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents() = 0;
	virtual void HandleKeyEvents(int key, bool bIsPressed) = 0;
	virtual void HandleTouchEvents(float x, float y, bool bIsPressed) = 0;
	virtual void HandleMouseMoveEvents(float x, float y) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	static std::shared_ptr<GSBase> CreateState(StateType stt);
	StateType GetGameStateType();

	void AddSoundByName(const char* soundName);
	void PlaySoundByName(const char* soundName, int channel, int loop);
	void PauseSoundByName(const char* soundName);
	void ResumeSoundByName(const char* soundName);
	void StopSoundByName(const char* soundName, int channel);

	void	AddAnimation(const char* name);
	void	DrawAnimation(const char* name);

	void	AddText(const char* name);
	void	RenderText(const char* name);
	void	RenderTextVector(std::vector<std::shared_ptr<Text>> textVector);
	void	UpdateText(const char* name, const char* message, float deltaTime);
	void	UpdateText(const char* name, float message, float deltaTime);

	void	DrawVectorObject(std::vector<std::shared_ptr<Object>> objectVector);
protected:
	StateType	m_stateType;
	std::map<std::string, std::shared_ptr<Sound>> m_soundMap;
	std::map<std::string, std::shared_ptr<Animation>> m_animationMap;
	std::map<std::string, std::shared_ptr<Text>> m_textMap;

	std::vector<int> soundChannelStates;

};

