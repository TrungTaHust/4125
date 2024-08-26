#pragma once
#include "GameObject/Animation.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "Globals.h"

class BaseAlien : public Animation
{
public:
	BaseAlien(
		std::shared_ptr<Model> model,
		std::vector<std::shared_ptr<Texture>> textureVector,
		std::shared_ptr<Shaders> shader,
		std::vector<GLint> numFrames,
		std::vector<GLint> numActions,
		std::vector<GLint> currentAction,
		std::vector<GLfloat> frameTime,
		float speed,
		int health,
		int score
	);
	BaseAlien();
	BaseAlien(const BaseAlien& other);
	virtual ~BaseAlien();

	bool HandleTouchEvent(GLfloat x, GLfloat y, bool bIsPressed);
	
	void SetAliveStatus(bool status);
	bool GetAliveStatus() { return m_isAlive; };
	void SetAlienAnimation(int index);
	float GetSpeed() { return m_speed; };
	int GetScore() { return m_score; };
	int GetCoin() { return m_coin; };
	void SetCoin(int coin);

	void SetDeath();
	void TakeDmg(int dmg);
	bool CheckCollide(Vector2 targetPos, Vector2 targetSize);

	void UpdatePos(float deltaTime, float difficult);

	float m_destroyedTime = 0;
	float m_directionYTime = 0;
	int m_direction = 0;
private:
	bool m_isAlive;
	bool m_isHover;
	bool m_isClick;
	float m_speed;
	int m_health;
	int m_score;
	int m_coin{};
};

