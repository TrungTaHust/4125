#include "../TrainingFramework/stdafx.h"
#include "GSPlay.h"
#include "GSOver.h"
 
std::vector<int> Globals::topScores;

GSPlay::GSPlay()
{
	m_stateType = STATE_PLAY;
}

GSPlay::~GSPlay()
{
}

void GSPlay::Init()
{
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("play_background"));
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("human_base"));
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("play_upper_pane"));
	m_objectVector.push_back(SceneManager::GetInstance()->GetObjectByID("play_lower_pane"));

	m_hearts.push_back(SceneManager::GetInstance()->GetObjectByID("heart1"));
	m_hearts.push_back(SceneManager::GetInstance()->GetObjectByID("heart2"));
	m_hearts.push_back(SceneManager::GetInstance()->GetObjectByID("heart3"));
	m_hearts.push_back(SceneManager::GetInstance()->GetObjectByID("heart4"));
	AddAnimation("loading_animation");
	AddAnimation("coins");
	
	wall = SceneManager::GetInstance()->GetAnimationByID("lightning_wall");

	m_buttonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_pause"));
	
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_resume_2"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_home"));
	m_pauseButtonList.push_back(SceneManager::GetInstance()->GetButtonByID("button_tutorial"));

	m_upgradeVector.push_back(SceneManager::GetInstance()->GetButtonByID("bullet_amount"));
	m_upgradeVector.push_back(SceneManager::GetInstance()->GetButtonByID("freeze"));
	m_upgradeVector.push_back(SceneManager::GetInstance()->GetButtonByID("fire"));
	m_upgradeVector.push_back(SceneManager::GetInstance()->GetButtonByID("health_upgrade"));

	m_upgradeTextCostVector.push_back(ResourceManager::GetInstance()->GetTextPointerByName("bullet_cost"));
	m_upgradeTextCostVector.push_back(ResourceManager::GetInstance()->GetTextPointerByName("freeze_cost"));
	m_upgradeTextCostVector.push_back(ResourceManager::GetInstance()->GetTextPointerByName("fire_cost"));
	m_upgradeTextCostVector.push_back(ResourceManager::GetInstance()->GetTextPointerByName("health_cost"));

	AddText("reloading");
	AddText("current_bullets");
	AddText("scores");
	AddText("gsplay_scores");
	AddText("your_bullets");
	AddText("coins");

	AddText("bullet_cost");
	AddText("freeze_cost");
	AddText("fire_cost");
	AddText("health_cost");

	AddSoundByName("play");
	AddSoundByName("explosion");
	AddSoundByName("freeze");
	AddSoundByName("fire");
	AddSoundByName("more");
	AddSoundByName("reload");

	PlaySoundByName("play", 7, -1);

	alienCount = 0;
	m_time = 1;
	lives = 3;
	maxLives = 3;
	alienSpawned = 0;
	score = 0;
	coin = 0;
	m_currentBullets = 10;
	m_maxBullets = 10;
	m_reloadTime = 2.0f;
	isBonusHealthUsed = false;
	isBulletOut = false;
	isFreezed = false;
	isFireSpellCasted = false;
	freezeTime = 3.0f;
	maxFreezeTime = 3.0f;
	fireTime = 3.0f;
	maxFireTime = 3.0f;
	fireCount = 0;
	bulletCost = 50;
	bonusHealthCost = 50;
	freezeCost = 50;
	fireCost = 50;
}

void GSPlay::Exit()
{
	StopSoundByName("explosion", 2);
	StopSoundByName("more", 3);
	StopSoundByName("freeze", 4);
	StopSoundByName("fire", 5);
	StopSoundByName("more", 6);
	StopSoundByName("play", 7);
	StopSoundByName("reload", 8);
}

void GSPlay::Pause()
{	
	for (int i = 2; i < 9; i++) {
		if (Mix_Playing(i)) {
			Mix_Pause(i);
			soundChannelStates.push_back(i);
		}
	}
}

void GSPlay::Resume()
{
	for (int channel : soundChannelStates) {
		Mix_Resume(channel);
	}
	soundChannelStates.clear();
}

void GSPlay::GunUpdate(float deltaTime) 
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

void GSPlay::SpawnFire()
{
	while (fireCount <=7) 
	{
		auto fire = SceneManager::GetInstance()->GetAnimationByID("fire");
		auto newFire = std::make_shared<Animation>(*fire);

		int posX = rand() % 1001 + 200;
		int posY = rand() % 361 + 300;
		newFire->Set2DPos(posX, posY);
		newFire->SetAnimation(0);
		m_fire.push_back(newFire);
		fireCount++;
	}
}

void GSPlay::AlienUpdate(float deltaTime)
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

void GSPlay::Update(float deltaTime)
{
	UpdateText("current_bullets", m_currentBullets, deltaTime);
	UpdateText("scores", score, deltaTime);
	UpdateText("coins", coin, deltaTime);

	UpdateText("bullet_cost", bulletCost, deltaTime);
	UpdateText("freeze_cost", freezeCost, deltaTime);
	UpdateText("fire_cost", fireCost, deltaTime);
	UpdateText("health_cost", bonusHealthCost, deltaTime);
		
	Vector4 yellow = { 255, 255, 0, 255 };
	Vector4 red = { 255, 0, 0, 255 };
	if (coin < bulletCost)
	{
		m_upgradeVector[0]->SetAlpha(0.5f);
		m_upgradeTextCostVector[0]->SetTextColor(red);
	}
	else
	{
		m_upgradeVector[0]->SetAlpha(1.0f);
		m_upgradeTextCostVector[0]->SetTextColor(yellow);
	}
	if (coin < freezeCost) 
	{
		m_upgradeVector[1]->SetAlpha(0.5f);
		m_upgradeTextCostVector[1]->SetTextColor(red);
	}
	else
	{
		m_upgradeVector[1]->SetAlpha(1.0f);
		m_upgradeTextCostVector[1]->SetTextColor(yellow);
	}
	if (coin < fireCost)
	{
		m_upgradeVector[2]->SetAlpha(0.5f);
		m_upgradeTextCostVector[2]->SetTextColor(red);
	}
	else
	{
		m_upgradeVector[2]->SetAlpha(1.0f);
		m_upgradeTextCostVector[2]->SetTextColor(yellow);
	}
	if (coin < bonusHealthCost && !isBonusHealthUsed)
	{
		m_upgradeVector[3]->SetAlpha(0.5f);
		m_upgradeTextCostVector[3]->SetTextColor(red);
	}
	else
	{
		m_upgradeVector[3]->SetAlpha(1.0f);
		m_upgradeTextCostVector[3]->SetTextColor(yellow);
	}

	for (int i = 0; i < maxLives; i++)
		if (i < lives) m_hearts[i]->SetTexture("red_heart");
		else m_hearts[i]->SetTexture("black_heart");

	UpdateDifficult();
	GunUpdate(deltaTime);
	if (isFireSpellCasted)
	{
		SpawnFire();
		fireTime -= deltaTime;
		if (fireTime <= 2.9f)
			for (auto fire : m_fire)
				fire->SetAnimation(1);

		if (fireTime <= 0.06f)
			for (auto fire : m_fire)
				fire->SetAnimation(2);

		if (fireTime <= 0)
		{
			fireTime = maxFireTime;
			isFireSpellCasted = false;
			m_fire.clear();
			fireCount = 0;
		}
		for (auto fire : m_fire)
			fire->Update(deltaTime);

		for (auto alien : m_alien)
			for (auto fire : m_fire)
				if (alien->GetAliveStatus() && alien->CheckCollide(fire->GetPos(), fire->GetSize()))
				{
					alien->SetDeath();
					alienCount--;
					score += alien->GetScore();
				}
	}

	SpawnByDifficult(deltaTime);
	for (auto bullet : m_bullets)
		bullet->Update(deltaTime);
	if (!isFreezed)	AlienUpdate(deltaTime);
	else
	{
		freezeTime -= deltaTime;
		if (freezeTime <= 0) 
		{
			freezeTime = maxFreezeTime;
			isFreezed = false;
		}
	}
		
	if (lives == 0)
	{
		UpdateScore(Globals::topScores, score); 
		for (int i = 2; i<= 8; i++)	Mix_HaltChannel(i);
		//GSMachine::GetInstance()->PopState();
		GSMachine::GetInstance()->PushState(StateType::STATE_GAMEOVER); 
	}
	for (auto& it : m_animationMap) 
		it.second->Update(deltaTime);

	wall->Update(deltaTime);
}

void GSPlay::Draw()
{
	DrawVectorObject(m_objectVector);
	for (int i = 0; i < 3; i++)
		m_upgradeVector[i]->Draw();
				
	RenderText("bullet_cost");
	RenderText("freeze_cost");
	RenderText("fire_cost");

	if (!isBonusHealthUsed)
	{
		m_upgradeVector[3]->Draw();
		RenderText("health_cost");
	}
	for (int i = 0; i < maxLives; i++)
		m_hearts[i]->Draw();
	wall->Draw();
	RenderText("current_bullets");
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

	if (isFireSpellCasted)
		for (auto fire : m_fire)
			fire->Draw();

	if (!GSMachine::GetInstance()->IsRunning())
	{
		SceneManager::GetInstance()->GetObjectByID("pause_frame")->Draw();
		for (auto button : m_pauseButtonList)
			button->Draw();
	}
}

void GSPlay::HandleEvents()
{
}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	if (bIsPressed && GSMachine::GetInstance()->IsRunning()) {
		switch (key) {
		case KEY_1:
			if (coin >= bulletCost)
			{
				PlaySoundByName("more", 3, 0);
				coin -= bulletCost;
				m_maxBullets += 2;
				m_currentBullets += 2;
				bulletCost += 20;
			}
			break;
		
		case KEY_2:
			if (coin >= freezeCost)
			{
				PlaySoundByName("freeze", 4, 0);
				coin -= freezeCost;
				isFreezed = true;
				maxFreezeTime *= 0.95;
				freezeCost += 20;
			}
			break;

		case KEY_3:
			if (coin >= fireCost)
			{
				PlaySoundByName("fire", 5, 0);
				coin -= fireCost;
				isFireSpellCasted = true;
				maxFireTime *= 0.95;
				fireCost += 20;
			}
			break;

		case KEY_4:
			if (coin >= bonusHealthCost && !isBonusHealthUsed)
			{
				PlaySoundByName("more", 6, 0);
				coin -= bonusHealthCost;
				isBonusHealthUsed = true;
				maxLives++;
				lives++;
			}
			break;
		}
	}
}

void GSPlay::HandleTouchEvents(float x, float y, bool bIsPressed)
{	
	if(!isBulletOut && y >= 100 && y <= 860 && GSMachine::GetInstance()->IsRunning() && bIsPressed)
	{
		m_currentBullets -= 1;
		m_bullets.push_back(SceneManager::GetInstance()->GetObjectByID("laser_bullet"));

		for (auto& alien : m_alien)
		{
			if (alien->HandleTouchEvent(x, y, bIsPressed))
			{
				alienCount--;
				score += alien->GetScore();
				coin += alien->GetCoin();
				PlaySoundByName("explosion", 2, 0);
			}
		}
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

void GSPlay::HandleMouseMoveEvents(float x, float y)
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

void GSPlay::Spawn(const char* type) 
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

void GSPlay::UpdateDifficult()
{
	if (alienSpawned == 200) return;
	m_mobAlienRate = 100 - alienSpawned / 2.0;
	m_medAlienRate = 0 + alienSpawned / 3.0;
	m_highAlienRate = 0 + alienSpawned / 6.0;
}

void GSPlay::SpawnByDifficult(float deltaTime) {
	m_time -= deltaTime;
	if (m_time <= 0) {
		m_time += 1;
		int randomNum = rand() % 100;
		const char* difficult = nullptr;
		if (randomNum <= m_mobAlienRate) difficult = "mob";
		else if (randomNum <= m_mobAlienRate + m_medAlienRate) difficult = "med";
		else difficult = "high";
		Spawn(difficult);
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