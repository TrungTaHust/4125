#pragma once
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"
#include <memory>
#include <vector>
#include <string>

class Object {

public:
	Object();
	~Object();

	Object(
		std::shared_ptr<Model> modelPtr,
		std::shared_ptr<Texture> texturePtr,
		std::shared_ptr<Shaders> shaderPtr
	);

	Object(
		const char* modelName,
		const char* textureName,
		const char* shaderName
	);

	void Draw();
	void Key();
	void Update(GLfloat deltaTime);

	void SetPos(Vector3 pos);
	void SetScale(Vector3 scale);
	void SetRotate(Vector3 rotate);
	void SetCamera(std::shared_ptr<Camera> cameraPtr);
	void SetSize(GLfloat width, GLfloat height);
	void SetObjectID(std::string objectID);
	void SetAlpha(GLfloat alpha);
	void Set2DPos(GLfloat x, GLfloat y);
	void Set2DPosV(Vector2 m_Pos);
	void SetTexture(const char* textureName);
	bool CheckCollide(std::shared_ptr<Object> target);
	Vector2 GetPos() { return Vector2(m_pos.x, m_pos.y); }
	Vector2 GetSize() const {
		return Vector2(m_width, m_height);
	}

	std::string GetObjectID();
	
	Matrix CalculateWVP();

	std::string getObjectID() { return m_objectID; }
	std::shared_ptr <Model> getModel() { return m_model; }
	std::shared_ptr <Texture> getTexture() { return m_texture; }
	std::shared_ptr <Shaders> getShader() { return m_shader; }
	std::shared_ptr <Camera> getCamera() { return m_camera; }

	Vector3 getPos() { return m_pos; }
	Vector3 getScale() { return m_scale; }
	Vector3 getRot() { return m_rotate; }

	GLfloat getAlpha() { return m_alpha; }
	GLfloat getWidth() { return m_width; }
	GLfloat getHeight() { return m_height; } 

	bool HandleTouchEvent(GLfloat x, GLfloat y, bool bIsPressed);
	void HandleMoveEvent(GLfloat x, GLfloat y);

protected:
	std::string m_objectID;
	std::shared_ptr <Model> m_model;
	std::shared_ptr <Texture> m_texture;
	std::shared_ptr <Shaders> m_shader;
	std::shared_ptr <Camera> m_camera;

	Vector3 m_pos;
	Vector3 m_scale;
	Vector3 m_rotate;

	GLfloat m_alpha;
	GLfloat m_width;
	GLfloat m_height;

	bool m_isHover;
	bool m_isClick;
};

