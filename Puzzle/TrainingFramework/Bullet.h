#pragma once
#include "GameObject/Object.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "Globals.h"

class Bullet : public Object
{
public:
	Bullet(
		const char* model,
		const char* texture,
		const char* shader
	);
	Bullet();
	Bullet(Object& other);
	virtual ~Bullet();

	void SetTarget(int targetX, int targetY);
	void SetSpeed(double speed);
	double GetSpeed() { return m_speed; };
	int GetBulletDmg() { return m_dmg; };

	bool CheckCollide(Vector2 targetPos, Vector2 targetSize);

	void UpdatePos(float deltaTime);
	void NewBullet();

private:
	bool m_isHover{};
	bool m_isClick{};
	bool setTarget;
	float m_speed = 900;
	Vector2 m_normDirection;

	int m_targetX;
	int m_targetY;

	int m_dmg = 1;
};

