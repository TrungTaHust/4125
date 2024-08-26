#include "../TrainingFramework/stdafx.h"
#include "GameButton.h"

GameButton::GameButton(
	const char* modelName,
	const char* textureName,
	const char* shaderName,
	const char* type
)
{
	auto model = ResourceManager::GetInstance()->GetModelPointerByName(modelName);
	auto texture = ResourceManager::GetInstance()->GetTexturePointerByName(textureName);
	auto shader = ResourceManager::GetInstance()->GetShaderPointerByName(shaderName);

	m_type = StringToButtonType(type);

	m_model = model;
	m_texture = texture;
	m_shader = shader;

	m_isHover = false;
	m_isClick = false;
}

GameButton::GameButton()
{
	m_isHover = false;
	m_isClick = false;
	m_type = DEFAULT_BUTTON;
}

GameButton::~GameButton()
{
}

bool GameButton::HandleTouchEvent(GLfloat x, GLfloat y, bool bIsPressed)
{
	if ((m_pos.x - m_width / 2.0f <= x) && (x <= m_pos.x + m_width / 2.0f)
		&& (m_pos.y - m_height / 2.0f <= y) && (y <= m_pos.y + m_height / 2.0f)
		&& bIsPressed)
	{
		m_alpha = 2.0f;
		return true;
	}

	if (!m_isClick) m_alpha = 1.0f;

	return false;
}

void GameButton::HandleMoveEvent(GLfloat x, GLfloat y)
{
	if ((m_pos.x - m_width / 2.0f <= x) && (x <= m_pos.x + m_width / 2.0f)
		&& (m_pos.y - m_height / 2.0f <= y) && (y <= m_pos.y + m_height / 2.0f))
	{
		m_isHover = true;
		if (!m_isClick) m_alpha = 0.8f;
	}
	else
	{
		m_alpha = 1.0f;
		if (!m_isClick) m_alpha = 1.0f;
	}
}

bool GameButton::IsHover() 
{
	return m_isHover;
}

ButtonType GameButton::StringToButtonType(const char* buttonTypeStr) {
	static const std::unordered_map<std::string, ButtonType> typeMap = {
		{"BUTTON_START", BUTTON_START},
		{"BUTTON_EXIT", BUTTON_EXIT},
		{"BUTTON_BACK_TO_MENU", BUTTON_BACK_TO_MENU},
		{"BUTTON_SCORE", BUTTON_SCORE},
		{"BUTTON_PAUSE", BUTTON_PAUSE},
		{"BUTTON_RESUME", BUTTON_RESUME},
		{"BUTTON_RESTART", BUTTON_RESTART},
		{"BUTTON_UPGRADE_BULLET_AMOUNT", BUTTON_UPGRADE_BULLET_AMOUNT},
		{"BUTTON_UPGRADE_HEALTH", BUTTON_UPGRADE_HEALTH},
		{"BUTTON_SKILL_FREEZE", BUTTON_SKILL_FREEZE},
		{"BUTTON_SKILL_FIRE", BUTTON_SKILL_FIRE},
		{"BUTTON_BACK", BUTTON_BACK},
		{"BUTTON_ENDLESS", BUTTON_ENDLESS},
		{"BUTTON_TUTORIAL", BUTTON_TUTORIAL},
		{"BUTTON_MAP1", BUTTON_MAP1}, 
		{"BUTTON_MAP2", BUTTON_MAP2}, 
		{"BUTTON_MAP3", BUTTON_MAP3}, 
		{"BUTTON_MAP4", BUTTON_MAP4}, 
		{"BUTTON_MAP5", BUTTON_MAP5}, 
		{"BUTTON_MAP6", BUTTON_MAP6} 
		
	};

	auto it = typeMap.find(buttonTypeStr);
	if (it != typeMap.end()) {
		return it->second;
	}
}