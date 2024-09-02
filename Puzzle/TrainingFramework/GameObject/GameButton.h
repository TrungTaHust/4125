#pragma once
#include "Object.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include <map>
#include <unordered_map>

enum ButtonType 
{
	BUTTON_START,
	BUTTON_EXIT,
	BUTTON_BACK_TO_MENU,
	BUTTON_SCORE,
	BUTTON_PAUSE,
	BUTTON_RESUME,
	BUTTON_RESTART,
	DEFAULT_BUTTON,
	BUTTON_UPGRADE_BULLET_AMOUNT,
	BUTTON_UPGRADE_HEALTH,
	BUTTON_SKILL_FREEZE,
	BUTTON_SKILL_FIRE,
	BUTTON_BACK,
	BUTTON_TUTORIAL,
	BUTTON_ENDLESS,
	BUTTON_MAP1,
	BUTTON_MAP2,
	BUTTON_MAP3,
	BUTTON_MAP4,
	BUTTON_MAP5,
	BUTTON_MAP6
};
class GameButton : public Object
{
public:
	GameButton(
		const char* modelName,
		const char* textureName,
		const char* shaderName,
		const char* type
	);
	GameButton();
	~GameButton();

	bool HandleTouchEvent(GLfloat x, GLfloat y, bool bIsPressed);
	void HandleMoveEvent(GLfloat x, GLfloat y);
	
	bool IsHover();
	ButtonType GetButtonType() { return m_type; };
	ButtonType m_type;

	ButtonType StringToButtonType(const char* buttonTypeStr);

private:

	bool m_isHover;
	bool m_isClick;
};