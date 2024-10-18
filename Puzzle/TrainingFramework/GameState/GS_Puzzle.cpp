#include "../TrainingFramework/stdafx.h"
#include "GS_Puzzle.h"

GSPuzzle::GSPuzzle()
{
	m_stateType = STATE_PUZZLE;
}

GSPuzzle::~GSPuzzle()
{
}

void GSPuzzle::Init()
{
	int count = 0;
	srand(static_cast<unsigned>(time(0)));

	auto background = SceneManager::GetInstance()->GetObjectByID("map1");
	background->Set2DPos(640, 480);
	background->SetSize(1280, 960);
	m_objectVector.push_back(background);

	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_pause"));

	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_resume_2"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_home"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_tutorial"));

	// Tạo tên FILE từ a02 đến z02
	char letter = 'a';
	for (int i = 1; i <= 26; ++i) {
		fileMap[i] = std::string(1, letter) + "02";  // Tạo chuỗi FILE
		letter++;  // Chuyển sang ký tự tiếp theo
	}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "upgrade_frame", "TriangleShader");
			slot->Set2DPos(440 + j * 100, 180 + i * 100);
			slot->SetSize(80, 80);
			m_frame.push_back(slot);
		}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "red_rectangle", "TriangleShader");
			slot->Set2DPos(440 + j * 100, 180 + i * 100);
			slot->SetSize(72, 72);
			int randomNum =  rand() % 4 + 1;
			switch(randomNum) {
			case 1:
				slot->SetTexture("white_rectangle");
				break;
			case 2:
				slot->SetTexture("yellow_rectangle");
				break;
			case 3:
				slot->SetTexture("red_rectangle");
				break;
			case 4:
				slot->SetTexture("green_rect");
				break;
			}
			m_color.push_back(slot);
		}

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
			slot->Set2DPos(440 + j * 100, 180 + i * 100);
			slot->SetSize(50, 50);
			

			//// Gọi hàm getRandomNumber và in ra số ngẫu nhiên
			//int randomNum = rand() % 26 + 1;
			//std::string textureName = fileMap[randomNum];
			//slot->SetTexture(textureName.c_str());
			m_ans.push_back(slot);
		}

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 2; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
			slot->Set2DPos(190 + i * 100, 750 + j * 100);
			slot->SetSize(50, 50);


			//// Gọi hàm getRandomNumber và in ra số ngẫu nhiên
			int randomNum = rand() % 26 + 1;
			std::string textureName = fileMap[randomNum];
			slot->SetTexture(textureName.c_str());
			m_keyboard.push_back(slot);
		}

	//AddText("scores");

	AddSoundByName("play");
	AddSoundByName("explosion");
	AddSoundByName("reload");

	PlaySoundByName("play", 7, -1);

	m_time = 1;	
	score = 0;	
}

void GSPuzzle::Exit()
{
	StopSoundByName("explosion", 2);
	StopSoundByName("play", 7);
	StopSoundByName("reload", 8);
}

void GSPuzzle::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSPuzzle::Resume()
{
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSPuzzle::Update(float deltaTime)
{
	//UpdateText("scores", score, deltaTime);	
}

void GSPuzzle::Draw()
{
	DrawVectorObject(m_objectVector);
	
	//RenderText("scores");
	
	for (auto& button : m_buttonList)
		button->Draw();

	DrawVectorObject(m_color);
	DrawVectorObject(m_frame);
	DrawVectorObject(m_ans);
	DrawVectorObject(m_keyboard);


	if (!GSMachine::GetInstance()->IsRunning())
	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto button : m_pauseButtonList)
			button->Draw();
	}	
}

void GSPuzzle::HandleEvents()
{
}

void GSPuzzle::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSPuzzle::HandleTouchEvents(float x, float y, bool bIsPressed)
{	
	if (GSMachine::GetInstance()->IsRunning())
	{
		for (auto& button : m_buttonList) {
			if (button->HandleTouchEvent(x, y, bIsPressed))
			{
				switch (button->m_type)
				{
				case BUTTON_PAUSE:
					GSMachine::GetInstance()->Pause();
					button->SetAlpha(0.5f);
					break;
				}
			};
		}

		for (auto& slot : m_ans) {
			if (slot->HandleTouchEvent(x, y, bIsPressed))
				if (strcmp(slot->getTexture()->GetID().c_str(), "null") != 0)
					slot->SetTexture("null");
		}

		for (auto& slot : m_keyboard) {
			if (slot->HandleTouchEvent(x, y, bIsPressed))
				if (count < 5)
				{
					m_ans[count]->SetTexture(slot->getTexture()->GetID().c_str());
					count++;
				}
				else count = 0;
		}
	}

	if (!GSMachine::GetInstance()->IsRunning())
	{
		for (auto& button : m_pauseButtonList) {
			if (button->HandleTouchEvent(x, y, bIsPressed))
			{
				GSMachine::GetInstance()->Resume();
				switch (button->m_type)
				{
				case BUTTON_RESUME:
					break;
				case BUTTON_BACK_TO_MENU:
					GSMachine::GetInstance()->PopState();
					break;
				case BUTTON_TUTORIAL:
					GSMachine::GetInstance()->PushState(STATE_TUTORIAL);
					break;
				}
			};
		}
	}
}

void GSPuzzle::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
	{
		button->HandleMoveEvent(x, y);
	}

	if (!GSMachine::GetInstance()->IsRunning())
	{
		for (auto& button : m_pauseButtonList)
		{
			button->HandleMoveEvent(x, y);
		}
	}
}