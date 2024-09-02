#pragma once
#include "../Utilities/Math.h"
#include <memory>
#include <string>
class Camera {
public:
	Camera();
	Camera(
		GLfloat fNear, 
		GLfloat fFar, 
		GLfloat fov, 
		GLfloat speed);

	~Camera();

	void CalculateWorldMatrix();
	void CalculateViewMatrix();
	void CalculateBasicVector();

	void Move(GLfloat deltaTime, int typeMove);
	void Rotate(GLfloat deltaTime, int typeRotate);
	
	void SetTarget(Vector3 target);
	void SetPosition(Vector3 pos);
	void SetCameraID(std::string cameraID);
	std::string GetCameraID();
	
	Matrix GetWorldMatrix();
	Matrix GetViewMatrix();
	Matrix GetProjectMatrix();
private:
	Matrix m_worldMatrix;
	Matrix m_viewMatrix;
	Matrix m_projectMatrix;

	Vector3 m_basicVector[3];
	Vector3 m_position;		
	Vector3 m_target;		
	Vector3 m_up;			

	float m_fov;
	float m_near = 0.1f;
	float m_far = 1000.0f;
	float m_moveSpeed = 3.0f;
	float m_rotateSpeed = 3.0f;
	std::string m_cameraID;
};