#pragma once
#include"GSBase.h"
#include"../TrainingFramework/GameManager/SingletonClass.h"
#include<list>
class GSMachine : public SingletonClass<GSMachine>
{
public:
	GSMachine();
	~GSMachine();

	void	Cleanup();
	void	ChangeState(StateType stt);
	void	PushState(StateType stt);
	void	PopState();
	void	PerformStateChange();

	void	Update(float deltaTime);
	void	Draw();
	bool	IsRunning();
	void	Pause();
	void	Resume();
	std::shared_ptr<GSBase>	GetCurrentState()
	{
		return m_pActiveState;
	}

private:
	std::list<std::shared_ptr<GSBase>>	m_StateStack;
	std::shared_ptr<GSBase>				m_pActiveState;
	std::shared_ptr<GSBase>				m_pNextState;

	bool m_running = true;
};

