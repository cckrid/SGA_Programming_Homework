#include "stdafx.h"
#include "MainGame22.h"
#include "GameScene1.h"
#include "NoneScene.h"

MainGame22::MainGame22()
{
}


MainGame22::~MainGame22()
{
}

HRESULT MainGame22::Init()
{
	GameNode::Init();
	isDebug = false;

	SCENE->AddScene("Minesweeper", new GameScene1);
	SCENE->AddScene("None", new NoneScene);

	SCENE->ChangeScene("None");

	bg = new Image;
	bg->Init("images/bg.bmp", WINSIZEX, WINSIZEY);

	icon1 = new Image;
	icon1->Init("images/icon1.bmp", 70, 65, true, RGB(0, 128, 128));

	rcMineStart = RectMake(143, 120, 73, 65 + 20);

	isGame = NULL;

	return S_OK;
}

void MainGame22::Release()
{
	GameNode::Release();
	SCENE->Release();
	SAFE_DELETE(bg);
}

void MainGame22::Update()
{
	GameNode::Update();

	SCENE->Update();

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		if (PtInRect(&rcMineStart, g_ptMouse) && isGame == false) {
			SCENE->ChangeScene("Minesweeper");
			isGame = true;
		}
	}

	if (SCENE->GetCurrentSceneName() == "None")
		isGame = false;

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame22::Render()
{
	PatBlt(GetMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		bg->Render(GetMemDC());

		icon1->Render(GetMemDC(), 145, 120);

		// ������ ������ ���� ����Ŀ��� ��� �ȹٲ�
		SetBkMode(GetMemDC(), TRANSPARENT);
		// ���ڻ� ����
		SetTextColor(GetMemDC(), RGB(255, 255, 255));

		HFONT hFont = CreateFont(17, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
		HFONT OldFont = (HFONT)SelectObject(GetMemDC(), hFont);

		sprintf_s(str, "����ã��");
		TextOut(GetMemDC(), 143, 120 + icon1->GetHeight(), str, strlen(str));

		SelectObject(GetMemDC(), OldFont);
		DeleteObject(hFont);

		if(isGame)
			SCENE->Render();
	}
	//==================   Debug   ====================
	if (isDebug)
	{
		sprintf_s(str, "x : %d, y : %d", g_ptMouse.x, g_ptMouse.y);
		TextOut(GetMemDC(), 10, 10, str, strlen(str));

		RectangleMake(GetMemDC(), rcMineStart);
		
	}
	//=================================================
	this->SetBackBuffer()->Render(GetHDC());
}
