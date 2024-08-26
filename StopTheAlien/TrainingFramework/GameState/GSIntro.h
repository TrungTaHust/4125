#pragma once
#include "../TrainingFramework/stdafx.h"
#include "GSBase.h"
#include "../TrainingFramework/GameObject/Object.h"
#include "../TrainingFramework/GameObject/Sound.h"
class GSIntro :
    public GSBase
{
public:
	GSIntro();
	~GSIntro();

	void	Init() override;
	void	Exit() override;
	void	Pause() override;
	void	Resume() override;
	void	Update(float deltaTime) override;
	void	Draw() override;


	void	HandleEvents() override;
	void	HandleKeyEvents(int key, bool bIsPressed) override;
	void	HandleTouchEvents(float x, float y, bool bIsPressed) override;
	void	HandleMouseMoveEvents(float x, float y) override;

private:
	std::shared_ptr<Object> m_intro;
	float	m_ftime;
	std::shared_ptr<Sound> m_introSound;
};

