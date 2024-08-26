#include "../TrainingFramework/stdafx.h"
#include "GS_Campaign_1.h"
 
GSCampaign1::GSCampaign1()
{
	m_stateType = STATE_CAMPAIGN_1;
}

GSCampaign1::~GSCampaign1()
{
}

void GSCampaign1::Init()
{
	auto background = SceneManager::GetInstance()->GetObjectByID("map1");
	background->Set2DPos(640, 480);
	background->SetSize(1280, 960);
	m_objectVector.push_back(background);
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("human_base"));
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("play_upper_pane"));
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("play_lower_pane"));

	m_gun.push_back(SceneManager::GetInstance()->GetObjectByID("z_gun"));
	m_bulletTexture.push_back(SceneManager::GetInstance()->GetObjectByID("z_laser_bullet"));

	m_hearts.push_back(SceneManager::GetInstance()->GetObjectByID("heart1"));
	m_hearts.push_back(SceneManager::GetInstance()->GetObjectByID("heart2"));
	m_hearts.push_back(SceneManager::GetInstance()->GetObjectByID("heart3"));
	AddAnimation("loading_animation");
	AddAnimation("coins");

	wall = SceneManager::GetInstance()->GetAnimationByID("lightning_wall");

	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_pause"));

	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_resume_2"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_home"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_tutorial"));

	AddText("reloading");
	AddText("current_bullets");
	AddText("scores");
	AddText("gsplay_scores");
	AddText("your_bullets");
	AddText("coins");

	AddSoundByName("play");
	AddSoundByName("explosion");
	AddSoundByName("reload");

	PlaySoundByName("play", 7, -1);

	alienCount = 0;
	m_time = 1;
	lives = 3;
	maxLives = 2;
	alienSpawned = 0;
	score = 0;
	coin = 0;
	m_currentBullets = 10;
	m_maxBullets = 10;
	m_reloadTime = 2.0f;
	
	isBulletOut = false;
	
}

void GSCampaign1::Exit()
{
	StopSoundByName("explosion", 2);
	StopSoundByName("play", 7);
	StopSoundByName("reload", 8);
}

void GSCampaign1::Pause()
{
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSCampaign1::Resume()
{
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSCampaign1::GunUpdate(float deltaTime)
{
	if (isBulletOut) {
		m_reloadTime -= deltaTime;
		if (m_reloadTime <= 0) {
			m_reloadTime = 2.0f;
			m_currentBullets = m_maxBullets;
			isBulletOut = false;
		}
	}
	else
	{
		if (m_currentBullets == 0)	isBulletOut = true;
		PlaySoundByName("reload", 8, 0);
	}
}

void GSCampaign1::AlienUpdate(float deltaTime)
{
	std::vector<std::shared_ptr<BaseAlien>> aliveAlien;
	float difficult = (10.0 + alienSpawned % 10) / 10;
	for (auto& alien : m_alien) {
		if (alien->GetAliveStatus())
		{
			alien->UpdatePos(deltaTime, difficult);
			if (alien->CheckCollide(wall->GetPos(), wall->GetSize())) {
				alien->SetDeath();
				alienCount--;
				lives--;
				PlaySoundByName("explosion", 2, 0);
			}

			aliveAlien.push_back(alien);
		}
		else
		{
			alien->m_destroyedTime += deltaTime;
			if (alien->m_destroyedTime <= 0.5f)
				aliveAlien.push_back(alien);
		}
		alien->Update(deltaTime);
	}
	m_alien = aliveAlien;
}

void GSCampaign1::BulletUpdate(float deltaTime)
{
	std::vector<std::shared_ptr<Bullet>> currentBullets;
	for (auto& bullet : m_bullets) {
		auto pos = bullet->GetPos();
		if (pos.x > 0 && (pos.y - 85 * sin(bullet->getScale().z)) > 100 && (pos.y + 85 * sin(bullet->getScale().z)) < 860)
		{
			bullet->UpdatePos(deltaTime);
			currentBullets.push_back(bullet);
		}
	}
	m_bullets = currentBullets;
}

void GSCampaign1::Update(float deltaTime)
{
	std::string current_bullets = "Bullets: " + std::to_string(m_currentBullets);
	UpdateText("your_bullets", current_bullets.c_str(), deltaTime);
	UpdateText("scores", score, deltaTime);
	UpdateText("coins", coin, deltaTime);

	for (int i = 0; i < maxLives; i++)
		if (i < lives) m_hearts[i]->SetTexture("red_heart");
		else m_hearts[i]->SetTexture("black_heart");

	GunUpdate(deltaTime);

	SpawnByDifficult(deltaTime);

	BulletUpdate(deltaTime);
	AlienUpdate(deltaTime);
	
	//Check if bullet hit alien
	for (auto alien : m_alien)
	{
		std::vector<std::shared_ptr<Bullet>> m_bulletsShot;
		for (auto bullet : m_bullets)
			if (alien->GetAliveStatus() && alien->CheckCollide(bullet->GetPos(), bullet->GetSize()))
			{
				alien->TakeDmg(bullet->GetBulletDmg());
				if (!alien->GetAliveStatus())
				{
					alien->SetDeath();
					alienCount--;
					score += alien->GetScore();
					coin += alien->GetCoin();
					PlaySoundByName("explosion", 2, 0);
				}
			}
			else m_bulletsShot.push_back(bullet);
		m_bullets = m_bulletsShot;
	}

	//Check lives
	if (lives == 0)
	{
		for (int i = 2; i <= 8; i++)	Mix_HaltChannel(i);
		GSMachine::GetInstance()->PushState(StateType::STATE_GAMEOVER);
	}
	for (auto& it : m_animationMap)
		it.second->Update(deltaTime);

	wall->Update(deltaTime);
}

void GSCampaign1::Draw()
{
	DrawVectorObject(m_objectVector);

	for (int i = 0; i < maxLives; i++)
		m_hearts[i]->Draw();
	wall->Draw();
	RenderText("your_bullets");
	RenderText("gsplay_scores");
	RenderText("scores");
	RenderText("coins");
	DrawAnimation("coins");
	for (auto& alien : m_alien)
		alien->Draw();

	for (auto& bullet : m_bullets)
		bullet->Draw();

	if (isBulletOut)
	{
		DrawAnimation("loading_animation");
		RenderText("reloading");
	}
	for (auto& button : m_buttonList)
		button->Draw();


	if (!GSMachine::GetInstance()->IsRunning())
	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto button : m_pauseButtonList)
			button->Draw();
	}
	m_gun[gun_index]->Draw();
}

void GSCampaign1::HandleEvents()
{
}

void GSCampaign1::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSCampaign1::HandleTouchEvents(float x, float y, bool bIsPressed)
{
	if (!isBulletOut && y >= 100 && y <= 860 && GSMachine::GetInstance()->IsRunning() && bIsPressed)
	{
		m_currentBullets -= 1;

		auto bullet = std::make_shared<Bullet>(*m_bulletTexture[gun_index]);

		bullet->SetTarget(x, y);
		bullet->Set2DPos(1200, 480);
		bullet->NewBullet();

		m_bullets.push_back(bullet);
	}

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

void GSCampaign1::HandleMouseMoveEvents(float x, float y)
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

	//Gun direction
	float a = y - 480;
	float c = x - 1200;
	double arctan = a / c;
	float result = atan(arctan);
	Vector3 direction;
	direction.z = result;
	m_gun[gun_index]->SetRotate(direction);
}

void GSCampaign1::Spawn(const char* type)
{
	if (alienCount == 10) return;
	std::string difficult(type);
	int randomAlien = rand() % 4 + 1;
	std::string alienName = difficult + "Alien" + std::to_string(randomAlien);

	auto new_alien = SceneManager::GetInstance()->GetAlienByID(alienName.c_str());
	auto alien = std::make_shared<BaseAlien>(*new_alien);
	float animationX = 50;
	float animationY = (float)Globals::screenHeight / 2 + (rand() % 5) * 120 - 240;
	alien->Set2DPos(animationX, animationY);

	m_alien.push_back(alien);
	alienCount++;
	alienSpawned++;
}

void GSCampaign1::SpawnByDifficult(float deltaTime) {
	m_time -= deltaTime;
	if (m_time <= 0) {
		m_time += 1;
		if(alienSpawned <= 15)
			Spawn("mob");
	}

	if (alienSpawned > 15 && m_alien.empty())
	{
		for (int i = 2; i <= 8; i++)	Mix_HaltChannel(i);
		std::ofstream outputFile("../TrainingFramework/map.txt");
		Globals::map[1] = '1';
		if (outputFile.is_open()) {
			outputFile << Globals::map << std::endl;
			outputFile.close();
		}
		GSMachine::GetInstance()->PushState(StateType::STATE_VICTORY);
	}
}