#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"

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

	IMAGE->AddImage("intro", "images/intro.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	IMAGE->AddImage("bg", "images/background.bmp", 0, -WINSIZEY,
		WINSIZEX, WINSIZEY * 2, false, RGB(255, 0, 255));

	IMAGE->AddImage(
		"rocket", "images/rocket.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));
	IMAGE->AddImage(
		"shadow", "images/shadow.bmp", WINSIZEX / 2, WINSIZEY - 200,
		52, 64, true, RGB(255, 0, 255));

	IMAGE->AddImage("bullet", "images/bullet_blue_10x1.bmp", 0, 0,
		25 * 10, 25, 10, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("alien", "images/alien_idle_16x1.bmp", 0, 0,
		50 * 16, 50, 16, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("missile", "images/missile.bmp",
		0, 0, 416, 64, 13, 1, true, RGB(255, 0, 255));

	int temp = 100;

	// boss
	IMAGE->AddImage("boss_deploy", "images/boss/boss_deploy_15x1.bmp", 0, 0,
		temp / 4 * 15, 50, 15, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("boss_fire", "images/boss/boss_fire_18x1.bmp", 0, 0,
		temp * 2.5 * 18, 100, 18, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("boss_enemy", "images/boss/boss_enemy_9x1.bmp", 0, 0,
		60 * 9, 50, 9, 1, true, RGB(255, 0, 255));
	
	IMAGE->AddImage("boss_hatch", "images/boss/boss_hatch_18x1.bmp", 0, 0,
		temp * 18, 25, 18, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("boss_idle", "images/boss/boss_idle_24x1.bmp", 0, 0,
	300 * 24, 250, 24, 1, true, RGB(255, 0, 255));

	IMAGE->AddImage("gameover", "images/gameover.bmp", 0, 0,
		WINSIZEX, WINSIZEY, false, RGB(255, 0, 255));

	_em = new EnemyManager;
	_em->Init();

	_em->SetRocket((Rocket*)SCENE->AddScene("Rocket", new Rocket));

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

	offsetY = 0;

	isStart = false;
	isLoad = false;

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


	if (INPUT->GetKeyDown(VK_RETURN) && !isStart) {
		//isStart = true;
		//SCENE->ChangeScene("Rocket");
		isLoad = true;
	}

	if (isLoad) {
		loadCount++;

		_loading->SetGauge(loadCount, LOADMAX);
		_loading->Update();

		if (loadCount == LOADMAX) {
			isLoad = false;
			isStart = true;
			SCENE->ChangeScene("Rocket");
		}
	}


	if (isStart) {
		offsetY--;
	}

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
	this->SetBackBuffer()->Render(GetHDC());
}
