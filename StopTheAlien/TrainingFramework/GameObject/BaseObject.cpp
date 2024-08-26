#include "../TrainingFramework/stdafx.h"
#include "BaseObject.h"
#include "Vertex.h"
#include <memory>
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include <vector>

BaseObject::BaseObject()
{
	m_camera = SceneManager::GetInstance()->GetCamera("smth");
	m_alpha = 2.0f;
	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 1.0f);
	m_rotate = Vector3(0.0f, 0.0f, 0.0f);
}

BaseObject::BaseObject(
	std::shared_ptr<Model> modelPtr,
	std::vector<std::shared_ptr<Texture>> textureVector,
	std::shared_ptr<Shaders> shaderPtr)
{
	m_model = modelPtr;
	m_textureVector = textureVector;
	m_currentTexture = m_textureVector[0];
	m_shader = shaderPtr;

	m_camera = SceneManager::GetInstance()->GetCamera("smth");
	m_alpha = 2.0f;
	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 1.0f);
	m_rotate = Vector3(0.0f, 0.0f, 0.0f);
}

BaseObject::~BaseObject()
{
}

void BaseObject::SetPos(Vector3 pos)
{
	m_pos = pos;
}

void BaseObject::SetScale(Vector3 scale)
{
	m_scale = scale;
}

void BaseObject::SetRotate(Vector3 rotate)
{
	m_rotate = rotate;
}

Matrix BaseObject::CalculateWVP()
{
	Matrix Rx, Ry, Rz, R, S, T;
	Rx.SetRotationX(m_rotate.x);
	Ry.SetRotationY(m_rotate.y);
	Rz.SetRotationZ(m_rotate.z);

	R = Rz * Rx * Ry;
	S.SetScale(m_scale);
	T.SetTranslation(m_pos);
	Matrix worldMatrix = S * R * T;
	return worldMatrix * m_camera->GetViewMatrix() * m_camera->GetProjectMatrix();
};

void BaseObject::SetCamera(std::shared_ptr<Camera> cameraPtr) {
	m_camera = cameraPtr;
};

void BaseObject::SetSize(GLfloat width, GLfloat height)
{
	m_width = width;
	m_height = height;
	SetScale(Vector3(width, height, 1.0f));

}

void BaseObject::SetObjectID(std::string objectID)
{
	m_objectID = objectID;
};

void BaseObject::SetAlpha(GLfloat alpha)
{
	m_alpha = alpha;
};

std::string BaseObject::GetObjectID()
{
	return m_objectID;
};

void BaseObject::Set2DPos(GLfloat x, GLfloat y)
{
	m_pos = Vector3(x, y, 0.0f);
}

void BaseObject::Set2DPosV(Vector2 m_Pos) {
	m_pos = Vector3(m_Pos.x, m_Pos.y, 0.0f);
}
