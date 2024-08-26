#include "../TrainingFramework/stdafx.h"
#include "GSMachine.h"
#include "GSBase.h"

GSMachine::GSMachine()
{
}

GSMachine::~GSMachine()
{
}

void GSMachine::Cleanup()
{
	while (!m_StateStack.empty()) {
		m_StateStack.back()->Exit();
		m_StateStack.pop_back();
	}
}

void GSMachine::ChangeState(StateType stt)
{
	std::shared_ptr<GSBase> nextState = GSBase::CreateState(stt);
	m_pNextState = nextState;
}

void GSMachine::PushState(StateType stt)
{
	std::shared_ptr<GSBase> nextState = GSBase::CreateState(stt);

	if (!m_StateStack.empty()) {
		m_StateStack.back()->Pause();
	}

	m_pNextState = nextState;
}

void GSMachine::PopState()
{
	if (!m_StateStack.empty()) {
		m_StateStack.back()->Exit();
		m_StateStack.pop_back();
	}

	if (!m_StateStack.empty()) {
		m_StateStack.back()->Resume();
		m_pActiveState = m_StateStack.back();
	}


}

void GSMachine::PerformStateChange()
{
	if (m_pNextState != 0)
	{
		if (!m_StateStack.empty())
		{
			m_pActiveState->Pause();
		}
		m_pNextState->Init();
		m_pActiveState = m_pNextState;
		m_StateStack.push_back(m_pNextState);
	}

	m_pNextState = 0;
}

void GSMachine::Update(float deltaTime)
{
	if (m_StateStack.size() != 0)
	{
		m_pActiveState->Update(deltaTime);
	}
}

void GSMachine::Draw()
{
	if (m_StateStack.size() != 0)
	{
		m_pActiveState->Draw();
	}
}

bool	GSMachine::IsRunning()
{
	return m_running;
}

void	GSMachine::Pause()
{
	if (IsRunning())
	{
		// Trò chơi đang chạy, tạm dừng nó và cài đặt trạng thái pause
		m_running = false;
		if (m_pActiveState)
		{
			m_pActiveState->Pause();
		}
	}
}

void	GSMachine::Resume()
{
	if (!IsRunning())
	{
		// Trò chơi đã tạm dừng, tiếp tục nó và cài đặt trạng thái running
		m_running = true;
		if (m_pActiveState)
		{
			m_pActiveState->Resume();
		}
	}
}