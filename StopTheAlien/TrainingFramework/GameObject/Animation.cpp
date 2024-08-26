#include "../TrainingFramework/stdafx.h"
#include "Animation.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "Vertex.h"

Animation::Animation(
	std::shared_ptr<Model> model, 
	std::vector<std::shared_ptr<Texture>> textureVector,
	std::shared_ptr<Shaders> shader,
	std::vector<GLint> numFrames,
	std::vector<GLint> numActions,
	std::vector<GLint> currentAction,
	std::vector<GLfloat> frameTime
) :
	BaseObject(model, textureVector, shader),
	m_numFramesVector(numFrames),
	m_numActionsVector(numActions),
	m_currentActionVector(currentAction),
	m_frameTimeVector(frameTime)
{
	m_currentFrame = 0;
	m_currentTime = 0;
	m_camera = SceneManager::GetInstance()->GetCamera("smth");

	m_pos = Vector3(0.0f, 0.0f, 0.0f);
	m_scale = Vector3(1.0f, 1.0f, 1.0f);
	m_rotate = Vector3(0.0f, 0.0f, 0.0f);

	m_numFrames = m_numFramesVector[0];
	m_numActions = m_numActionsVector[0];
	m_currentAction = m_currentActionVector[0];
	m_frameTime = m_frameTimeVector[0];

	m_currentTexture = m_textureVector[0];
}

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Draw()
{
	glUseProgram(m_shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, m_model->getVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->getIboId());
	glBindTexture(GL_TEXTURE_2D, m_currentTexture->getTextureId());


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
		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(Vector3)));
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

	GLint iTempShaderVaribleGLID = glGetUniformLocation(m_shader->program, "u_numFrames");
	if (iTempShaderVaribleGLID != -1)
	{
		glUniform1f(iTempShaderVaribleGLID, m_numFrames);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = glGetUniformLocation(m_shader->program, "u_currentFrame");
	if (iTempShaderVaribleGLID != -1)
	{
		glUniform1f(iTempShaderVaribleGLID, m_currentFrame);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = glGetUniformLocation(m_shader->program, "u_numActions");
	if (iTempShaderVaribleGLID != -1)
	{
		glUniform1f(iTempShaderVaribleGLID, m_numActions);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = glGetUniformLocation(m_shader->program, "u_currentAction");
	if (iTempShaderVaribleGLID != -1)
	{
		glUniform1f(iTempShaderVaribleGLID, m_currentAction);
	}

	glDrawElements(GL_TRIANGLES, m_model->getNumIndices(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Animation::Update(float deltatime)
{
	m_currentTime += deltatime;
	if (m_currentTime >= m_frameTime)
	{
		m_currentFrame++;
		if (m_currentFrame >= m_numFrames)
			m_currentFrame = 0;
		m_currentTime -= m_frameTime;
	}
}

void Animation::SetAnimation(int index)
{
	m_currentTexture = m_textureVector[index];
	m_numFrames = m_numFramesVector[index];
	m_frameTime = m_frameTimeVector[index];
	m_numActions = m_numActionsVector[index];
	m_currentAction = m_currentActionVector[index];
};
