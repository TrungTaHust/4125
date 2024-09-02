#pragma once
#include "../TrainingFramework/GameState/GSBase.h"
#include "../TrainingFramework/GameObject/GameButton.h"
#include "../TrainingFramework/BaseAlien.h"
class GSTutorial :
    public GSBase
{
public:
    GSTutorial();
    ~GSTutorial();

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
	std::shared_ptr<Object>						m_tutorialBackground;
	std::vector<std::shared_ptr<Animation>>		m_tutorialAnimation;
	std::vector<std::shared_ptr<Object>>		m_objectVector;
	std::vector<std::shared_ptr<GameButton>>	m_buttonList;
	std::shared_ptr<Text>						m_text;
	std::vector<std::shared_ptr<Text>>			m_textVector;
};

