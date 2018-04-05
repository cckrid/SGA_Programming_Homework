#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"
#include "ResultScene.h"

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	GameNode::Init();
	isDebug = false;

	SOUND->Init();
	// ���� �߰�
	// bgm
	SOUND->AddSound("bgm", "sounds/bgm.mp3", true, true);
	// intro
	SOUND->AddSound("intro", "sounds/intro.mp3", true, true);
	// ending
	SOUND->AddSound("ending", "sounds/ending.mp3", true, true);
	// gameover
	SOUND->AddSound("gameover", "sounds/gameover.mp3", true, true);
	// boss_bgm
	SOUND->AddSound("boss_bgm", "sounds/boss_bgm.mp3", true, true);

	// rocket_fire
	SOUND->AddSound("rocket_fire", "sounds/rocket_fire.wav", false, false);
	// rocket_damaged
	SOUND->AddSound("rocket_damaged", "sounds/rocket_damaged.wav", false, false);
	// alien_fire
	SOUND->AddSound("alien_fire", "sounds/alien_fire.wav", false, false);
	// alien_die
	SOUND->AddSound("alien_die", "sounds/alien_damaged.wav", false, false);
	// �Ⱦ���� ���� ������ Ÿ�̹��̳� �̰� ������
	// boss_fire
	SOUND->AddSound("boss_fire", "sounds/boss_fire.mp3", false, false);
	// boss_damaged
	SOUND->AddSound("boss_damaged", "sounds/boss_damaged.wav", false, false);

	// �̹��� �߰�
	// ���� ����ȭ��
	IMAGE->AddImage("intro", "images/intro.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	// ���� ���ȭ��
	IMAGE->AddImage("bg", "images/background.bmp", 0, -WINSIZEY,
		WINSIZEX, WINSIZEY * 2, false, RGB(255, 0, 255));

	// ���� ���� ȭ��
	IMAGE->AddImage("gameover", "images/gameover.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	// ���� ȭ��
	IMAGE->AddImage("ending", "images/ending.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	// �÷��̾� ����
	IMAGE->AddImage(
		"rocket", "images/rocket.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));
	// ���� �׸���
	IMAGE->AddImage(
		"shadow", "images/shadow.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));

	// �⺻ �Ѿ�
	IMAGE->AddImage("bullet", "images/bullet_blue_10x1.bmp", 0, 0,
		25 * 10, 25, 10, 1, true, RGB(255, 0, 255));

	// alien idle
	IMAGE->AddImage("alien_idle", "images/alien_idle_16x1.bmp", 0, 0,
		50 * 16, 50, 16, 1, true, RGB(255, 0, 255));
	// alien died
	IMAGE->AddImage("alien_died", "images/alien_died_21x1.bmp", 0, 0,
		75 * 21, 75, 21, 1, true, RGB(255, 0, 255));

	// �÷��̾� �Ѿ� (�̻���)
	IMAGE->AddImage("missile", "images/missile.bmp",
		0, 0, 416, 64, 13, 1, true, RGB(255, 0, 255));

	// ��ġ ���� ���ϰ� �ϱ� ���� �ӽ� ���� ��
	int temp = 100;

	// boss �̹��� 
	// ���� ��
	IMAGE->AddImage("boss_deploy", "images/boss/boss_deploy_15x1.bmp", 0, 0,
		temp / 4 * 15, 50, 15, 1, true, RGB(255, 0, 255));

	// ���� �� �߻�
	IMAGE->AddImage("boss_fire", "images/boss/boss_fire_18x1.bmp", 0, 0,
		temp * 2.5 * 18, 100, 18, 1, true, RGB(255, 0, 255));

	// ���� ���ʹ� ��ȯ
	IMAGE->AddImage("boss_enemy", "images/boss/boss_enemy_9x1.bmp", 0, 0,
		60 * 9, 50, 9, 1, true, RGB(255, 0, 255));
	
	// ���� ��ġ (
	IMAGE->AddImage("boss_hatch", "images/boss/boss_hatch_18x1.bmp", 0, 0,
		temp * 18, 25, 18, 1, true, RGB(255, 0, 255));

	// ���� idle
	IMAGE->AddImage("boss_idle", "images/boss/boss_idle_24x1.bmp", 0, 0,
	300 * 24, 250, 24, 1, true, RGB(255, 0, 255));

	// save book
	IMAGE->AddImage("save_book", "images/savebook_9x1.bmp", 0, 0,
		50 * 9, 50, 9, 1, true, RGB(255, 0, 255));

	_em = new EnemyManager;
	_em->Init();

	Rocket* rocket = (Rocket*)SCENE->AddScene("Rocket", new Rocket);
	rocket->SetEnemyManager(_em); // enemyManager rocket�� �����ͷ� ����

	_em->SetRocket(rocket); // rocket enemyManger�� �����ͷ� ����

	_loading = new ProgressBar;
	//_loading->Init((char*)"images/bar_front.bmp", (char*)"images/bar_back.bmp", 
	//	WINSIZEX / 2 - 350, WINSIZEY - 100, 700, 50);

	_loading->Init(
		IMAGE->AddImage(
			"loading_front", "images/bar_front.bmp", 0, 0, 700, 50, true, RGB(255,0,255)),
		IMAGE->AddImage(
			"loading_back", "images/bar_back.bmp", 0, 0, 700, 50, true, RGB(255,0,255)),
		WINSIZEX / 2 - 350, WINSIZEY - 100, 700, 50);

	loadCount = 0;

	//SCENE->ChangeScene("Rocket");

	//SCENE->AddScene("Test", new TestScene);

	//SCENE->ChangeScene("Test");

	// ��� ��
	ResultScene* scene = (ResultScene*)SCENE->AddScene("Result", new ResultScene);
	scene->SetImage(IMAGE->FindImage("gameover"));
	scene->SetEnemyManager(_em);	// ��� ���� enemyManger �����ͷ� ����
	scene->SetMainGame(this);		// ��� ���� mainGame �����ͷ� ����

	_em->SetResultScene(scene);		// enemyManger�� ����� �����ͷ� ����

	rocket->SetResultScene(scene);

	offsetY = 0;

	isStart = false;
	isLoad = false;

	SOUND->Play("intro", 0.5f);

	SAVEBOOK->Init();

	return S_OK;
}

void MainGame::Release()
{
	GameNode::Release();
	_em->Release();
	SAFE_DELETE(_em);
}

void MainGame::Update()
{
	GameNode::Update();

	if(isStart)
		_em->Update();

	SCENE->Update();


	// ����Ű ������ �ε��� ����
	if (INPUT->GetKeyDown(VK_RETURN) && !isStart) {
		//isStart = true;
		//SCENE->ChangeScene("Rocket");
		isLoad = true;
	}

	if (isLoad) {
		loadCount++;

		_loading->SetGauge(loadCount, LOADMAX);
		_loading->Update();

		// �ε��� ������ ���� ������ �� ����
		if (loadCount == LOADMAX) {
			isLoad = false;
			isStart = true;
			SCENE->ChangeScene("Rocket");

			if (SOUND->IsPlaySound("intro"))
				SOUND->Stop("intro");
		}
	}


	if (isStart) {
		offsetY--;
	}

	SAVEBOOK->Update();

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame::Render()
{
	PatBlt(GetMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		if (isStart) {
			IMAGE->FindImage("bg")->LoopRender(GetMemDC(),
				&RectMake(0, 0, WINSIZEX, WINSIZEY), 0, offsetY);
		}
		else
			IMAGE->FindImage("intro")->Render(GetMemDC());

		if (isLoad) {
			_loading->Render();
		}

		SCENE->Render();

		if(isStart)
			_em->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	if(isStart)
		SAVEBOOK->Render(GetMemDC());
	FRAME->Render(GetMemDC());
	this->SetBackBuffer()->Render(GetHDC());
}

void MainGame::ReStart()
{
	offsetY = 0;
	_em->Init();

	isStart = false;
	isLoad = true;

	loadCount = 0;

	IMAGE->FindImage("rocket")->SetX(WINSIZEX / 2);
	IMAGE->FindImage("rocket")->SetY(WINSIZEY - 200);

	if (SOUND->IsPlaySound("ending"))
		SOUND->Stop("ending");
	if (SOUND->IsPlaySound("gameover"))
		SOUND->Stop("gameover");

	SOUND->Play("intro", 0.5f);
}
