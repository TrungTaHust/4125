#pragma once
#include "BaseObject.h"
class Animation :  public BaseObject
{
public:
	Animation(
		std::shared_ptr<Model> model, 
		std::vector<std::shared_ptr<Texture>> textureVector,
		std::shared_ptr<Shaders> shader,
		std::vector<GLint> numFrames, 
		std::vector<GLint> numActions,
		std::vector<GLint> currentAction,
		std::vector<GLfloat> frameTime
	);

	Animation(const Animation& other)
	{
		this->m_objectID = other.m_objectID;
		this->m_model = other.m_model;
		this->m_currentTexture = other.m_currentTexture;
		this->m_textureVector = other.m_textureVector;
		this->m_shader = other.m_shader;
		this->m_camera = other.m_camera;

		this->Set2DPos(other.m_pos.x, other.m_pos.y);
		this->SetSize(other.m_scale.x, other.m_scale.y);

		this->m_alpha = other.m_alpha;
		this->m_width = other.m_width;
		this->m_height = other.m_height;

		this->m_numFrames = other.m_numFrames;
		this->m_currentFrame = other.m_currentFrame;
		this->m_frameTime = other.m_frameTime;
		this->m_currentTime = other.m_currentTime;
		this->m_numActions = other.m_numActions;
		this->m_currentAction = other.m_currentAction;

		this->m_numFramesVector = other.m_numFramesVector;
		this->m_frameTimeVector = other.m_frameTimeVector;
		this->m_numActionsVector = other.m_numActionsVector;
		this->m_currentActionVector = other.m_currentActionVector;
	}

	Animation();
	~Animation() override;
	Vector2 GetSize() const {
		return Vector2(m_width, m_height); 
	}
	void Draw() override;
	void Update(float deltatime) override;

	void SetAnimation(int index);
	
protected:
	GLint			m_numFrames;
	GLint			m_currentFrame;
	GLfloat			m_frameTime;
	GLfloat			m_currentTime;
	GLint			m_numActions;
	GLint			m_currentAction;

	std::vector<GLint> 			m_numFramesVector;
	std::vector<GLfloat>		m_frameTimeVector;
	std::vector<GLint> 			m_numActionsVector;
	std::vector<GLint> 			m_currentActionVector;
};

