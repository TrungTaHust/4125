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

void UpdateScore(std::vector<int>& scores, int currentScore);
class GSEndless :
    public GSBase
{
public:
	GSEndless();
	~GSEndless();

	void	Init() override;
	void	Exit() override;
	void	Pause() override;
	void	Resume() override;
	void	Update(float deltaTime) override;
	void	Draw() override;

	void	Spawn(float deltaTime);
	void	AlienUpdate(float deltaTime);
	void	BulletUpdate(float deltaTime);

	void	HandleEvents() override;
	void	HandleKeyEvents(int key, bool bIsPressed) override;
	void	HandleTouchEvents(float x, float y, bool bIsPressed) override;
	void	HandleMouseMoveEvents(float x, float y) override;
	
private:
	std::vector<std::shared_ptr<Object>> m_objectVector;
	std::vector<std::shared_ptr<BaseAlien>> m_alien;
	std::vector<std::shared_ptr<Bullet>> m_bullets;
	std::vector<std::shared_ptr<GameButton>> m_buttonList;
	std::shared_ptr<Animation> wall;
	std::vector<std::shared_ptr<GameButton>> m_pauseButtonList;
	std::shared_ptr<Object> m_gun;
	std::shared_ptr<Object> m_bulletTexture;
	std::shared_ptr<Object> m_aim;
	int alienCount;
	float m_time;

	int alienSpawned;
	int score;

	int coin;
};