#pragma once
#include "GSBase.h"
#include "../TrainingFramework/GameObject/Object.h"
#include "../TrainingFramework/GameObject/Animation.h"
#include "../TrainingFramework/GameObject/GameButton.h"
#include "../TrainingFramework/GameObject/Text.h"
#include "../BaseAlien.h"
#include "../Bullet.h"
#include "GSMachine.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include <cstdlib>

class GSPuzzle :
	public GSBase
{
public:
	GSPuzzle();
	~GSPuzzle();

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

	void	UpdateWord(std::vector<std::string>& wordVector);

private:
	std::vector<std::shared_ptr<Object>> m_objectVector;
	std::vector<std::shared_ptr<GameButton>> m_buttonList;
	std::vector<std::shared_ptr<GameButton>> m_pauseButtonList;
	std::vector<std::shared_ptr<Object>> m_frame;
	std::vector<std::shared_ptr<Object>> m_ans;
	std::vector<std::shared_ptr<Object>> m_color;
	std::vector<std::shared_ptr<Object>> m_keyboard;
	
	std::map<int, std::string> fileMap;

	std::string answer;
	std::string key;
	std::string check;
	
	std::vector<std::string> wordVector;

	float m_time;
	int score;
	int row_count;
	int col_count;
};