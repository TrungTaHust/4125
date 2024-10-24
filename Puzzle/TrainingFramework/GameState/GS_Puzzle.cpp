﻿#include "../TrainingFramework/stdafx.h"
#include "GS_Puzzle.h"

GSPuzzle::GSPuzzle()
	: row_count(0), col_count(0), score(0), answer("     "), check("00000")
{
	m_stateType = STATE_PUZZLE;
	// Tạo tên FILE từ a02 đến z02
	for (char letter = 'a'; letter <= 'z'; ++letter) {
		fileMap[letter - 'a' + 1] = std::string(1, letter) + "02";  // Tạo chuỗi FILE
	}

	// Tạo tên FILE từ a01 đến z01
	for (char letter = 'a'; letter <= 'z'; ++letter) {
		fileMapKeyboard[letter - 'a' + 1] = std::string(1, letter) + "01";  // Tạo chuỗi FILE
	}

	UpdateWord(wordVector);
}


GSPuzzle::~GSPuzzle()
{
}

void GSPuzzle::Init()
{
	srand(static_cast<unsigned>(time(0)));
	key = wordVector[rand() % wordVector.size()];

	auto sceneManager = SceneManager::GetInstance();
	m_objectVector.push_back(sceneManager->GetObjectByID("puzzle_background"));

	m_buttonList = {
		sceneManager->GetButtonByID("button_pause"),
		sceneManager->GetButtonByID("button_end"),
		sceneManager->GetButtonByID("button_check")
	};

	m_pauseButtonList = {
		sceneManager->GetButtonByID("button_resume_2"),
		sceneManager->GetButtonByID("button_home"),
		sceneManager->GetButtonByID("button_tutorial")
	};

	//Word Frame
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "upgrade_frame", "TriangleShader");
			slot->Set2DPos(300 + j * 100, 180 + i * 100);
			slot->SetSize(80, 80);
			m_frame.push_back(slot);
		}

	//Hint
	
	hint_picture = std::make_shared<Object>("Sprite2D", "white_rectangle", "TriangleShader");
	hint_picture->Set2DPos(1000, 280);
	hint_picture->SetSize(320, 220);
	m_frame.push_back(hint_picture);	

	//Answer color
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "white_rectangle", "TriangleShader");
			slot->Set2DPos(300 + j * 100, 180 + i * 100);
			slot->SetSize(72, 72);			
			m_color.push_back(slot);
		}

	//Answer 
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
			slot->Set2DPos(300 + j * 100, 180 + i * 100);
			slot->SetSize(50, 50);
			m_ans.push_back(slot);
		}

	//Keyboard
	for (int i = 0; i < 13; i++)
		for (int j = 0; j < 2; j++) {
			auto slot = std::make_shared<Object>("Sprite2D", "null", "TriangleShader");
			slot->Set2DPos(160 + i * 80, 750 + j * 100);
			slot->SetSize(50, 50);			
			slot->SetTexture(fileMapKeyboard[j * 13 + i + 1].c_str());
			m_keyboard.push_back(slot);
		}

	AddText("scores");

	AddSoundByName("play");
	AddSoundByName("explosion");
	AddSoundByName("reload");

	PlaySoundByName("play", 7, -1);

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

void GSPuzzle::Resume() {
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSPuzzle::Update(float deltaTime) {
	UpdateText("scores", score, deltaTime);	
	if (col_count == 4)
		hint_picture->SetTexture(key.c_str());
	else 
		hint_picture->SetTexture("white_rectangle");
}

void GSPuzzle::Draw(){
	DrawVectorObject(m_objectVector);
	
	RenderText("scores");
	
	for (auto& button : m_buttonList)
		button->Draw();

	DrawVectorObject(m_color);
	DrawVectorObject(m_frame);
	DrawVectorObject(m_ans);
	DrawVectorObject(m_keyboard);


	if (!GSMachine::GetInstance()->IsRunning())	{
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

void GSPuzzle::HandleTouchEvents(float x, float y, bool bIsPressed) {
	if (GSMachine::GetInstance()->IsRunning()) {
		for (auto& button : m_buttonList) {
			if (button->HandleTouchEvent(x, y, bIsPressed)) {
				switch (button->m_type) {
				case BUTTON_PAUSE:
					GSMachine::GetInstance()->Pause();
					button->SetAlpha(0.5f);
					break;
				case BUTTON_CHECK:
					if (row_count == 5) {
						// Gán các giá trị từ texture vào answer
						for (int i = 0; i < 5; i++)
							answer[i] = m_ans[col_count * 5 + i]->getTexture()->GetID()[0];

						// Khởi tạo mảng check với giá trị 0
						for (int i = 0; i < 5; i++)
							check[i] = '0';

						// Khởi tạo mảng để đánh dấu các ký tự trong key đã được khớp
						bool used_in_key[5] = { false, false, false, false, false };

						// Kiểm tra các vị trí trùng khớp chính xác
						for (int i = 0; i < 5; i++)
							if (answer[i] == key[i]) {
								check[i] = '1'; // Đánh dấu khớp chính xác
								used_in_key[i] = true; // Đánh dấu ký tự này đã được dùng
							}
						// Kiểm tra các vị trí trùng khớp không chính xác
						for (int i = 0; i < 5; i++)
							if (check[i] == '0')
							{
								for (int j = 0; j < 5; j++)
									if (answer[i] == key[j] && i != j && !used_in_key[j])
									{
										check[i] = '2'; // Gán khớp không chính xác
										used_in_key[j] = true; // Đánh dấu ký tự đã được dùng
										break;
									}

								if (check[i] == '0')
									check[i] = '3';
							}


						for (int i = 0; i < 5; i++)
							switch (check[i]) {
							case '1':
								m_color[col_count * 5 + i]->SetTexture("green_rect");
								break;
							case '2':
								m_color[col_count * 5 + i]->SetTexture("yellow_rectangle");
								break;
							case '3':
								m_color[col_count * 5 + i]->SetTexture("red_rectangle");
								break;
							}

						row_count = 0;
						col_count++;
						if (check == "11111") {
							score += 100;
							col_count = 0;
							key = wordVector[rand() % wordVector.size()];
							for (int i = 0; i < 25; i++) {
								m_ans[i]->SetTexture("null");
								m_color[i]->SetTexture("white_rectangle");
							}
							//GSMachine::GetInstance()->PushState(STATE_VICTORY);
							break;
						}
						if (col_count == 5)
							GSMachine::GetInstance()->PushState(STATE_GAMEOVER);
						break;
					}
					break;
				case BUTTON_END:
					GSMachine::GetInstance()->PushState(STATE_GAMEOVER);
					break;
				};
			}
		}

		//Clear letter
		for (int i = 0; i < row_count; i++)
			if (m_ans[col_count * 5 + i]->HandleTouchEvent(x, y, bIsPressed))
				if (strcmp(m_ans[col_count * 5 + i]->getTexture()->GetID().c_str(), "null") != 0)
				{
					for (int j = i; j < row_count; j++)
						m_ans[col_count * 5 + j]->SetTexture("null");

					row_count = i;
				}
		
		//Add letter
		for (auto& slot : m_keyboard)
			if (slot->HandleTouchEvent(x, y, bIsPressed))
				if (row_count < 5) {
					std::string textureID = slot->getTexture()->GetID();
					if (textureID.size() > 2 && textureID.substr(textureID.size() - 2) == "01") 
						textureID.replace(textureID.size() - 2, 2, "02");

					m_ans[col_count * 5 + row_count]->SetTexture(textureID.c_str());
					row_count++;							
				}
	}
	else
	{
		for (auto& button : m_pauseButtonList)
			if (button->HandleTouchEvent(x, y, bIsPressed))
			{
				switch (button->m_type)
				{
				case BUTTON_RESUME:
					GSMachine::GetInstance()->Resume();
					break;
				case BUTTON_BACK_TO_MENU:
					GSMachine::GetInstance()->PopState();
					break;
				case BUTTON_TUTORIAL:
					GSMachine::GetInstance()->Resume();
					GSMachine::GetInstance()->PushState(STATE_TUTORIAL);
					break;
				}
			};
	}
}


void GSPuzzle::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);

	for (auto& button : m_keyboard)
		button->HandleMoveEvent(x, y);

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList)		
			button->HandleMoveEvent(x, y);		
}

void GSPuzzle::UpdateWord(std::vector<std::string>& wordVector) {
	std::ifstream inputFile("../TrainingFramework/word.txt");
	if (inputFile.is_open()) {
		wordVector.clear();
		std::string line;
		while (std::getline(inputFile, line)) {
			try {				
				wordVector.push_back(line); 
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Lỗi định dạng tệp tin: " << e.what() << std::endl;
			}
		}
		inputFile.close();
	}	
}