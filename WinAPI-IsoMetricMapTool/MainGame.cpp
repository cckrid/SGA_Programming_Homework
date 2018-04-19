#include "stdafx.h"
#include "MainGame.h"

#include "TestScene.h"

#include "IsoMap.h"

MainGame::MainGame()
{
#ifdef SUBWINOPEN
	SUBWIN->Init();
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����
}

MainGame::~MainGame()
{
}

HRESULT MainGame::Init()
{
	GameNode::Init();
	isDebug = false;

	IMAGE->AddImage("tile_grass", "images/tile_grass.bmp", 
		0, 0, 100, 65, true, RGB(255, 0, 255));
	IMAGE->AddImage("tile", "images/tile.bmp",
		0, 0, 100 * 8, (50 + 50) * 7, 8, 7, true, RGB(255, 0, 255));

	TestScene * test = new TestScene;
	SCENE->AddScene("Test", test);

	SOUND->Init();
	SOUND->AddSound("Test", "sounds/����3.wav", true, true);

	SCENE->AddScene("IsoMap", new IsoMap);

#ifdef SUBWINOPEN
	SUBWIN->SetScene(test);
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����

	SCENE->ChangeScene("IsoMap");

	return S_OK;
}

void MainGame::Release()
{
	GameNode::Release();
}

void MainGame::Update()
{
	GameNode::Update();
	SCENE->Update();

#ifdef SUBWINOPEN
	SUBWIN->Update();
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����

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
		SCENE->Render();

#ifdef SUBWINOPEN
		SUBWIN->Render();
#endif // SUBWINDOW �ʿ�� MainGame.h �ּ� ����
		
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	FRAME->Render(GetMemDC());
	this->SetBackBuffer()->Render(GetHDC());
}
