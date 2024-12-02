#include "../TrainingFramework/stdafx.h"
#include "Object.h"
#include "Vertex.h"
#include <memory>
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include <vector>

Object::Object()
{
	m_camera = SceneManager::GetInstance()->GetCamera("smth");
	m_alpha = 2.0f;
	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 1.0f);
	m_rotate = Vector3(0.0f, 0.0f, 0.0f);
}

Object::Object(
	std::shared_ptr<Model> modelPtr, 
	std::shared_ptr<Texture> texturePtr, 
	std::shared_ptr<Shaders> shaderPtr
)
{
	m_model = modelPtr;
	m_texture = texturePtr;
	m_shader = shaderPtr;

	m_camera = SceneManager::GetInstance()->GetCamera("smth");
	m_alpha = 2.0f;
	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 1.0f);
	m_rotate = Vector3(0.0f, 0.0f, 0.0f);
}

Object::~Object() 
{
}

Object::Object(
	const char* modelName,
	const char* textureName,
	const char* shaderName
) 
{
	auto model = ResourceManager::GetInstance()->GetModelPointerByName(modelName);
	auto texture = ResourceManager::GetInstance()->GetTexturePointerByName(textureName);
	auto shader = ResourceManager::GetInstance()->GetShaderPointerByName(shaderName);

	m_model = model;
	m_texture = texture;
	m_shader = shader;

	m_camera = SceneManager::GetInstance()->GetCamera("smth");
	m_alpha = 2.0f;
	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 1.0f);
	m_rotate = Vector3(0.0f, 0.0f, 0.0f);
}

void Object::Draw() 
{
	if (m_texture == nullptr) {
		std::cout << "Texture is null: " << m_objectID << std::endl;
		return;
	}
	glUseProgram(m_shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->getVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->getIboId());
	glBindTexture(GL_TEXTURE_2D, m_texture->getTextureId());
		
	//finding location of uniforms / attributes
	GLint positionAttribute = glGetAttribLocation(m_shader->program, "a_posL");
	if (positionAttribute != -1) 
	{
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	GLint uvAttribute = glGetAttribLocation(m_shader->program, "a_uv");
	if (uvAttribute != -1) 
	{
		glEnableVertexAttribArray(uvAttribute);
		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	GLint uniformTextureLocation = glGetUniformLocation(m_shader->program, "u_texture");
	if (uniformTextureLocation != -1) 
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(uniformTextureLocation, 0);
		
	}

	GLint uniformWVPLocation = glGetUniformLocation(m_shader->program, "u_wvp");
	if (uniformWVPLocation != -1)
	{
		Matrix WVP = CalculateWVP();
		glUniformMatrix4fv(uniformWVPLocation, 1, GL_FALSE, WVP.m[0]);
	}

	GLint uniformAlpha = glGetUniformLocation(m_shader->program, "u_alpha");
	if (uniformAlpha != -1)
	{
		glUniform1f(uniformAlpha, m_alpha);
	}

	glDrawElements(GL_TRIANGLES, m_model->getNumIndices(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

};

void Object::SetPos(Vector3 pos) 
{
	m_pos = pos;
	//CalculateWVP();
}

void Object::SetScale(Vector3 scale) 
{
	m_scale = scale;
	//CalculateWVP();
}

void Object::SetRotate(Vector3 rotate) 
{
	m_rotate = rotate;
	//CalculateWVP();
}

void Object::Key() 
{
};

void Object::Update(GLfloat deltaTime) 
{
	
};

Matrix Object::CalculateWVP() 
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

void Object::SetCamera(std::shared_ptr<Camera> cameraPtr) {
	m_camera = cameraPtr;
};

void Object::SetSize(GLfloat width, GLfloat height)
{
	m_width = width;
	m_height = height;	
	SetScale(Vector3(width, height, 1.0f));
	//CalculateWVP();
}

void Object::SetObjectID(std::string objectID)
{
	m_objectID = objectID;
};

void Object::SetAlpha(GLfloat alpha) 
{
	m_alpha = alpha;
};

std::string Object::GetObjectID()
{
	return m_objectID;
};

void Object::Set2DPos(GLfloat x, GLfloat y)
{
	m_pos = Vector3(x, y, 0.0f);
	//CalculateWVP();

}

void Object::Set2DPosV(Vector2 m_Pos) {
	m_pos = Vector3(m_Pos.x, m_Pos.y, 0.0f);
	//CalculateWVP();
}

void Object::SetTexture(const char* textureName)
{
	auto texture = ResourceManager::GetInstance()->GetTexturePointerByName(textureName);
	m_texture = texture;
}

bool Object::HandleTouchEvent(GLfloat x, GLfloat y, bool bIsPressed)
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

void Object::HandleMoveEvent(GLfloat x, GLfloat y)
{
	if ((m_pos.x - m_width / 2.0f <= x) && (x <= m_pos.x + m_width / 2.0f)
		&& (m_pos.y - m_height / 2.0f <= y) && (y <= m_pos.y + m_height / 2.0f)) 
		m_alpha = 0.8f;
	else
		m_alpha = 1.0f;		
}

bool Object::CheckCollide(std::shared_ptr<Object> target)
{
	float leftA = m_pos.x - m_width / 2;
	float rightA = m_pos.x + m_width / 2;
	float topA = m_pos.y - m_height / 2;
	float bottomA = m_pos.y + m_height / 2;

	float leftB = target->m_pos.x - target->m_width / 2;
	float rightB = target->m_pos.x + target->m_width / 2;
	float topB = target->m_pos.y - target->m_height / 2;
	float bottomB = target->m_pos.y + target->m_height / 2;

	if (rightA < leftB || leftA > rightB || bottomA < topB || topA > bottomB) {
		return false; 
	}
	return true; 
}
