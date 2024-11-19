#pragma once
#include "../TrainingFramework/GameState/GSBase.h"
#include "../TrainingFramework/GameObject/Object.h"
#include "../TrainingFramework/GameObject/Animation.h"
#include "../TrainingFramework/GameObject/GameButton.h"
#include "../TrainingFramework/GameObject/Text.h"

void GetScore();

class GSOver :
    public GSBase
{
public:
	GSOver();
	~GSOver();

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

	void	SetScore(int score);
private:
	std::shared_ptr<Object> m_overBackground;
	std::vector<std::shared_ptr<GameButton>> m_buttonList;
	Text m_text;
	int m_score;
};

