#include "../TrainingFramework/stdafx.h"
#include "Bullet.h"
#include "math.h"

Bullet::Bullet(
	const char* model,
	const char* texture,
	const char* shader
)
{
	m_model = ResourceManager::GetInstance()->GetModelPointerByName(model);
	m_texture = ResourceManager::GetInstance()->GetTexturePointerByName(texture);
	m_shader = ResourceManager::GetInstance()->GetShaderPointerByName(shader);

	m_isHover = false;
	m_isClick = false;
	setTarget = false;

	m_targetX = 0;
	m_targetY = 0;
}

Bullet::~Bullet()
{
}

void Bullet::SetTarget(int targetX, int targetY)
{
	m_targetX = targetX;
	m_targetY = targetY;
};

void Bullet::SetSpeed(double speed) {
	m_speed = speed;
}

Bullet::Bullet()
{
	m_isHover = false;
	m_isClick = false;
	setTarget = false;


	m_targetX = 0;
	m_targetY = 0;
}

Bullet::Bullet(Object& other)
{
	this->m_objectID = other.getObjectID();
	this->m_model = other.getModel();
	this->m_texture = other.getTexture();
	this->m_shader = other.getShader();
	this->m_camera = other.getCamera();

	this->Set2DPos(other.getPos().x, other.getPos().y); 
	this->SetSize(other.getScale().x, other.getScale().y); 

	this->m_alpha = other.getAlpha();
	this->m_width = other.getWidth();
	this->m_height = other.getHeight();
}

void Bullet::UpdatePos(float deltaTime)
{
	if (!setTarget) {
		float X = (m_targetX - m_pos.x);
		float Y = (m_targetY - m_pos.y);
		m_normDirection.x = X / sqrt(X * X + Y * Y);
		m_normDirection.y = Y / sqrt(X * X + Y * Y);
		setTarget = true;
	}

	m_pos.x += m_normDirection.x * m_speed * deltaTime;
	m_pos.y += m_normDirection.y * m_speed * deltaTime;

	return;
}

void Bullet::NewBullet() { 
	setTarget = false; 
	float a = m_targetY - 480;
	float c = m_targetX - 1200;
	double arctan = a / c;
	float result = atan(arctan);
	m_rotate.z = result;
}

bool Bullet::CheckCollide(Vector2 targetPos, Vector2 targetSize)
{
	if ((targetPos.x - targetSize.x / 2.0f <= m_pos.x) && (m_pos.x <= targetPos.x + targetSize.x / 2.0f)
		&& (targetPos.y - targetSize.y / 2.0f <= m_pos.y) && (m_pos.y <= targetPos.y + targetSize.y / 2.0f))
		return true;
	return false;
}