#include "../TrainingFramework/stdafx.h"
#include "GSEndless.h"
#include "GSOver.h"
#include "math.h"
 
std::vector<int> Globals::topScores;

namespace {
	const float RELOAD_TIME = 2.0f;
	const float DESTROYED_TIME_LIMIT = 0.5f;
	const int MAX_LIVES = 3;
	const int INITIAL_BULLETS = 10;
	const int SOUND_CHANNEL_PLAY = 7;
	const int SOUND_CHANNEL_EXPLOSION = 2;
	const int SOUND_CHANNEL_RELOAD = 8;
}

GSEndless::GSEndless()
	: alienCount(0), m_time(1), alienSpawned(0), score(0), coin(0) // Initialize uninitialized variables
{
	m_stateType = STATE_ENDLESS;
}

GSEndless::~GSEndless()
{
}

void GSEndless::Init()
{
	auto sceneManager = SceneManager::GetInstance();

	m_objectVector.push_back(sceneManager->GetObjectByID("play_background"));
	m_objectVector.push_back(sceneManager->GetObjectByID("human_base"));
	m_objectVector.push_back(sceneManager->GetObjectByID("play_upper_pane"));
	m_objectVector.push_back(sceneManager->GetObjectByID("play_lower_pane"));

	m_gun = sceneManager->GetObjectByID("z_gun");
	m_bulletTexture = sceneManager->GetObjectByID("z_laser_bullet");

	AddAnimation("coins");
	
	wall = sceneManager->GetAnimationByID("lightning_wall");

	m_buttonList.push_back(sceneManager->GetButtonByID("button_pause"));

	m_pauseButtonList.push_back(sceneManager->GetButtonByID("button_resume_2"));
	m_pauseButtonList.push_back(sceneManager->GetButtonByID("button_home"));
	m_pauseButtonList.push_back(sceneManager->GetButtonByID("button_tutorial"));

	auto resourceManager = ResourceManager::GetInstance();

	m_aim = sceneManager->GetObjectByID("aim"); 
	
	AddText("scores");
	AddText("gsplay_scores");
	AddText("coins");

	AddSoundByName("play");
	AddSoundByName("explosion");

	PlaySoundByName("play", SOUND_CHANNEL_PLAY, -1); 
}

void GSEndless::Exit()
{
	StopSoundByName("explosion", SOUND_CHANNEL_EXPLOSION);
	StopSoundByName("play", SOUND_CHANNEL_PLAY);
	StopSoundByName("reload", SOUND_CHANNEL_RELOAD);
}

void GSEndless::Pause()
{	
	soundChannelStates.clear();
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSEndless::Resume()
{
	for (int channel : soundChannelStates) 
		Mix_Resume(channel);	
	soundChannelStates.clear();
}

void GSEndless::AlienUpdate(float deltaTime)
{
	std::vector<std::shared_ptr<BaseAlien>> aliveAlien;
	float difficult = (10.0 + alienSpawned % 10) / 10;
	for (auto& alien : m_alien) {
		if (alien->GetAliveStatus()) {
			alien->UpdatePos(deltaTime, difficult);
			if (alien->CheckCollide(wall->GetPos(), wall->GetSize())) {
				alien->SetDeath();
				alienCount--;
				PlaySoundByName("explosion", 2, 0);
			}

			aliveAlien.push_back(alien);
		} else {
			alien->m_destroyedTime += deltaTime;
			if (alien->m_destroyedTime <= 0.5f)
				aliveAlien.push_back(alien);
		}
		alien->Update(deltaTime);
	}
	m_alien = aliveAlien;
}

void GSEndless::BulletUpdate(float deltaTime)
{
	std::vector<std::shared_ptr<Bullet>> currentBullets;
	for (auto& bullet : m_bullets) {
		auto pos = bullet->GetPos();
		if (pos.x > 0 && (pos.y - 85 * sin(bullet->getScale().z)) > 100 && (pos.y + 85 * sin(bullet->getScale().z)) < 860) {
			bullet->UpdatePos(deltaTime);
			currentBullets.push_back(bullet);
		}
	}
	m_bullets = currentBullets;
}

void GSEndless::Update(float deltaTime)
{
	UpdateText("scores", score, deltaTime);
	UpdateText("coins", coin, deltaTime);
			
	Vector4 yellow = { 255, 255, 0, 255 };
	Vector4 red = { 255, 0, 0, 255 };	

	Spawn(deltaTime);
    	
	BulletUpdate(deltaTime);
	AlienUpdate(deltaTime);	

	for (auto& alien : m_alien) {
		std::vector<std::shared_ptr<Bullet>> m_bulletsShot;

		for (auto& bullet : m_bullets) {
			if (alien->GetAliveStatus() && alien->CheckCollide(bullet->GetPos(), bullet->GetSize())) {
				alien->TakeDmg(bullet->GetBulletDmg());
				if (!alien->GetAliveStatus()) {
					alien->SetDeath();
					alienCount--;
					score += alien->GetScore();
					coin += alien->GetCoin();
					PlaySoundByName("explosion", 2, 0);
				}
			}
			else m_bulletsShot.push_back(bullet);	
		}
		m_bullets = m_bulletsShot;
	}

	//Check lives
	
	for (auto& it : m_animationMap) 
		it.second->Update(deltaTime);

	wall->Update(deltaTime);
}

void GSEndless::Draw()
{
	DrawVectorObject(m_objectVector);	
	
	wall->Draw();
	RenderText("gsplay_scores");
	RenderText("scores");
	RenderText("coins");
	DrawAnimation("coins");
	for (auto& alien : m_alien)
		alien->Draw();

	for (auto& bullet : m_bullets)
		bullet->Draw();
	
	for (auto& button : m_buttonList)
		button->Draw();

	if (!GSMachine::GetInstance()->IsRunning())
	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto &button : m_pauseButtonList)
			button->Draw();
	}
	m_gun->Draw();
	m_aim->Draw();
}

void GSEndless::HandleEvents()
{
}

void GSEndless::HandleKeyEvents(int key, bool bIsPressed) {
	if (bIsPressed && GSMachine::GetInstance()->IsRunning()) 
		switch (key) {
				
		}
}

void GSEndless::HandleTouchEvents(float x, float y, bool bIsPressed)
{	
	if(y >= 100 && y <= 860 && GSMachine::GetInstance()->IsRunning() && bIsPressed)
	{
		auto bullet = std::make_shared<Bullet>(*m_bulletTexture);
		bullet->SetTarget(x, y);
		bullet->Set2DPos(1200, 480);
		bullet->NewBullet();
		m_bullets.push_back(bullet);
	}

	else 
		for (auto& button : m_buttonList) {
		if (button->HandleTouchEvent(x, y, bIsPressed))
			switch (button->m_type)	{
			case BUTTON_PAUSE:
				GSMachine::GetInstance()->Pause();
				button->SetAlpha(0.5f);
				break;
			}		
	}

	if (!GSMachine::GetInstance()->IsRunning())
		for (auto& button : m_pauseButtonList) 
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

void GSEndless::HandleMouseMoveEvents(float x, float y)
{
	for (auto& button : m_buttonList)
		button->HandleMoveEvent(x, y);
	
	if (!GSMachine::GetInstance()->IsRunning())	
		for (auto& button : m_pauseButtonList)
		{
			button->HandleMoveEvent(x, y);
		}
	
	//Gun direction
	float a = y - 480;
	float c = x - 1200;
	double arctan = a / c;
	float result = atan(arctan);
	Vector3 direction;
	direction.z = result;
	m_gun->SetRotate(direction);
	m_aim->Set2DPos(x, y);
}

void GSEndless::Spawn(float deltaTime) {
	m_time -= deltaTime;
	if (m_time <= 0) {
		m_time += 1;		
		if (alienCount == 10) return;
		std::string alienName = "mobAlien1";

		auto new_alien = SceneManager::GetInstance()->GetAlienByID(alienName.c_str());
		auto alien = std::make_shared<BaseAlien>(*new_alien);
		float animationX = 50;
		float animationY = (float)Globals::screenHeight / 2 + (rand() % 5) * 120 - 240;
		alien->Set2DPos(animationX, animationY);

		m_alien.push_back(alien);
		alienCount++;
		alienSpawned++;
	}
}

void UpdateScore(std::vector<int>& scores, int currentScore) {
	std::ifstream inputFile("../TrainingFramework/high_scores.txt");
	if (inputFile.is_open()) {
		scores.clear();
		std::string line;
		while (std::getline(inputFile, line)) {
			if (line == "###") {
				break;
			}

			try {
				int score = std::stoi(line);
				scores.push_back(score); // Thêm điểm vào vector
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Lỗi định dạng tệp tin: " << e.what() << std::endl;
			}
		}

		inputFile.close();
	}
	else {
		std::cerr << "Không thể mở tệp tin." << std::endl;
		return;
	}
	scores.push_back(currentScore);
	std::sort(scores.rbegin(), scores.rend());

	if (scores.size() > 5) {
		scores.resize(5);
	}

	std::ofstream outputFile("../TrainingFramework/high_scores.txt");
	if (outputFile.is_open()) {
		for (const int score : scores) {
			outputFile << score << std::endl;
		}
		outputFile << "###" << std::endl;
		outputFile << currentScore << std::endl;
		outputFile.close();
	}
	else {
		std::cerr << "Không thể ghi tệp tin." << std::endl;
		return;
	}
}