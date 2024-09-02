#pragma once
#include "../TrainingFramework/stdafx.h"
#include <memory>

template <class T>
class SingletonClass {
public:
	static std::shared_ptr<T> GetInstance()	{
		if (s_instance == 0)
			s_instance = std::make_shared<T>();
		return s_instance;
	}

	static bool HasInstance()	{
		return s_instance != 0;
	}

	static void FreeInstance()	{
		if (s_instance)
			s_instance.reset();
	}

protected:
	SingletonClass() {}
	virtual ~SingletonClass() {
		s_instance.reset();
	}

private:
	static std::shared_ptr<T> s_instance;
};

template <class T> std::shared_ptr<T> SingletonClass<T>::s_instance = nullptr;