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

class GSCampaign4 :
	public GSBase
{
public:
	GSCampaign4();
	~GSCampaign4();

	void	Init() override;
	void	Exit() override;
	void	Pause() override;
	void	Resume() override;
	void	Update(float deltaTime) override;
	void	Draw() override;

	void	Spawn(const char* type);
	void	UpdateDifficult();
	void	SpawnByDifficult(float deltaTime);
	void	GunUpdate(float deltaTime);
	void	AlienUpdate(float deltaTime);
	void	BulletUpdate(float deltaTime);
	void	SpawnFire();

	void	HandleEvents() override;
	void	HandleKeyEvents(int key, bool bIsPressed) override;
	void	HandleTouchEvents(float x, float y, bool bIsPressed) override;
	void	HandleMouseMoveEvents(float x, float y) override;

private:
	std::vector<std::shared_ptr<Object>> m_objectVector;
	std::vector<std::shared_ptr<Object>> m_upgradeVector;
	std::vector<std::shared_ptr<Object>> m_hearts;
	std::vector<std::shared_ptr<BaseAlien>> m_alien;
	std::vector<std::shared_ptr<Bullet>> m_bullets;
	std::vector<std::shared_ptr<GameButton>> m_buttonList;
	std::vector<std::shared_ptr<Animation>> m_fire;
	std::shared_ptr<Animation> wall;
	std::vector<std::shared_ptr<GameButton>> m_pauseButtonList;
	std::vector<std::shared_ptr<Object>> m_gun;
	std::vector<std::shared_ptr<Object>> m_bulletTexture;

	int gun_index = 0;
	int alienCount;
	float m_time;
	int lives;
	int maxLives;
	int alienSpawned;
	int score;
	float m_mobAlienRate = 0.5;
	float m_medAlienRate;
	float m_highAlienRate;
	float m_currentBullets;
	float m_maxBullets;
	float m_reloadTime;
	bool isBonusHealthUsed;
	bool isBulletOut;
	bool isFreezed;
	bool isFireSpellCasted;
	int coin;
	float freezeTime;
	float maxFreezeTime;
	float fireTime;
	float maxFireTime;
	int fireCount;
	int bulletCost;
	int bonusHealthCost;
	int freezeCost;
	int fireCost;
};