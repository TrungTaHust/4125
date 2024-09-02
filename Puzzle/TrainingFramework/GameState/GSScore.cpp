#include "../TrainingFramework/stdafx.h"
#include "GSScore.h"
#include "../TrainingFramework/GameManager/SceneManager.h"
#include "../TrainingFramework/GameManager/ResourceManager.h"
#include "../TrainingFramework/Globals.h"
#include "../TrainingFramework/GameState/GSMachine.h"

GLfloat num = 290;

GSScore::GSScore()
{
	m_stateType = STATE_SCORE;
}

GSScore::~GSScore()
{
}

void GSScore::Init()
{
	m_scoreBackground = SceneManager::GetInstance()->GetObjectByID("over_background");
	m_scoreFrame = SceneManager::GetInstance()->GetObjectByID("score_frame");
	auto buttonBack = SceneManager::GetInstance()->GetButtonByID("button_back");
	m_buttonList.push_back(buttonBack);
	
	GetHighScore(textVector);
}

void GSScore::Exit()
{
}

void GSScore::Pause()
{
}

void GSScore::Resume()
{
}

void GSScore::Update(float deltaTime)
{

}

void GSScore::Draw()
{
	m_scoreBackground->Draw();
	m_scoreFrame->Draw();
	for (auto& button : m_buttonList)
		button->Draw();
	
	for (auto& tex : textVector)
	{
		tex->Draw();
	}
		
}


void GSScore::HandleEvents()
{
}

void GSScore::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSScore::HandleTouchEvents(float x, float y, bool bIsPressed)
{
	for (auto& button : m_buttonList) {
		if (button->HandleTouchEvent(x, y, bIsPressed))
		{
			switch (button->m_type)
			{
			case BUTTON_BACK:
				GSMachine::GetInstance()->PopState();
				break;
			}
		};
	}
}

void GSScore::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
	{
		button->HandleMoveEvent(x, y);
	}
}

void GSScore::GetHighScore(std::vector<std::shared_ptr<Text>>& textVector) {
	// Mở tệp "high_scores.txt" để đọc
	std::ifstream inputFile("../TrainingFramework/high_scores.txt");
	if (inputFile.is_open()) {
		textVector.clear(); // Xóa danh sách các đối tượng Text cũ

		std::string line;
		std::string highScoreText;
		int n = 1;
		while (std::getline(inputFile, line)) {
			if (line == "###") {
				break; // Kết thúc nếu gặp "###"
			}

			try {
				int score = std::stoi(line);
				// Tạo một đối tượng Text cho điểm số và thêm vào danh sách
				auto text = std::make_shared<Text>();
				highScoreText = "High Score #" + std::to_string(n) + ": " + std::to_string(score) + "\n";
				n = n + 1;
				text->SetPos({ 640, num, 1 }); // Đặt vị trí của văn bản
				text->GetTextColor() = SDL_Color({ 255, 0, 0, 255 }); // Đặt màu sắc của văn bản
				text->GetTextSize() = 30; // Đặt kích thước của văn bản
				text->GetTextMessage() = highScoreText;
				text->Init(fontPath);
				textVector.push_back(text);
				num += 50;
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Lỗi định dạng tệp tin: " << e.what() << std::endl;
			}
		}
		num = 290;
		inputFile.close();
	}
	else {
		std::cerr << "Không thể mở tệp tin." << std::endl;
	}
}



