#include "stdafx.h"
#include "GameScene2.h"


GameScene2::GameScene2()
{
}


GameScene2::~GameScene2()
{
}

HRESULT GameScene2::Init()
{
	map = new Image;
	map->Init("images/gomoku/map.bmp", 19 * 25 + 10, 19 * 25 + 10);

	black = new Image;
	black->Init("images/gomoku/black.bmp", 25, 25, true, RGB(240, 191, 112));

	white = new Image;
	white->Init("images/gomoku/white.bmp", 25, 25, true, RGB(240, 191, 112));
	
	menu = new Image;
	menu->Init("images/gomoku/menu.bmp", 19 * 25 + 10, 25, true, RGB(240, 191, 112));

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			// test
			//gomoku[i][j].status = GOMOKU_BLACK;
			gomoku[i][j].status = GOMOKU_EMPTY;
			gomoku[i][j].x = 466 + i * 25;
			gomoku[i][j].y = 102 + j * 25;
			gomoku[i][j].rc = RectMake(gomoku[i][j].x, gomoku[i][j].y,
				25, 25);
		}
	}

	currentGomoku = GOMOKU_BLACK;

	isOver = false;

	rcGomokuExit = RectMake(923, 75, 25, 25);

	return S_OK;
}

void GameScene2::Release()
{
	SAFE_DELETE(map);
	SAFE_DELETE(black);
	SAFE_DELETE(white);
	SAFE_DELETE(menu);
}

void GameScene2::Update()
{
	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		if (PtInRect(&rcGomokuExit, g_ptMouse)) {
			SCENE->ChangeScene("None");
			return;
		}

		if (isOver) {
			Init();
			return;
		}

		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 19; j++) {
				if (PtInRect(&gomoku[i][j].rc, g_ptMouse) &&
					gomoku[i][j].status == GOMOKU_EMPTY) {
					gomoku[i][j].status = currentGomoku;
					if (currentGomoku == GOMOKU_BLACK)
						currentGomoku = GOMOKU_WHITE;
					else if (currentGomoku == GOMOKU_WHITE)
						currentGomoku = GOMOKU_BLACK;

					// check
					CheckGomoku(i,j, gomoku[i][j].status);
				}
			}
		}
	}

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void GameScene2::Render()
{
	menu->Render(GetMemDC(), WINSIZEX / 2 - 50, 100 - 25);

	map->Render(GetMemDC(), WINSIZEX/2 - 50, 100);

	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			switch (gomoku[i][j].status)
			{
			case GOMOKU_EMPTY:
				break;
			case GOMOKU_BLACK:
				black->Render(GetMemDC(), gomoku[i][j].x, gomoku[i][j].y);
				break;
			case GOMOKU_WHITE:
				white->Render(GetMemDC(), gomoku[i][j].x, gomoku[i][j].y);
				break;
			}
		}
	}

	// test
	/*black->Render(GetMemDC());
	white->Render(GetMemDC(),25, 0);*/

	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
	HFONT OldFont = (HFONT)SelectObject(GetMemDC(), hFont);

	if (isOver) {
		switch (winner)
		{
		case GOMOKU_EMPTY:
			sprintf_s(str, "    DRAW");
			break;
		case GOMOKU_BLACK:
			sprintf_s(str, "BLACK WIN");
			break;
		case GOMOKU_WHITE:
			sprintf_s(str, "WHITE WIN");
			break;
		}

		TextOut(GetMemDC(), 568, 273, str, strlen(str));

		sprintf_s(str, "Any Click");
		TextOut(GetMemDC(), 575, 353, str, strlen(str));

		sprintf_s(str, "ReStart");
		TextOut(GetMemDC(), 600, 403, str, strlen(str));
	}

	SelectObject(GetMemDC(), OldFont);
	DeleteObject(hFont);

	//==================   Debug   ====================
	if (isDebug)
	{
		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 19; j++) {
				RectangleMake(GetMemDC(), gomoku[i][j].rc);
			}
		}
		RectangleMake(GetMemDC(), rcGomokuExit);
	}
	//=================================================
}

void GameScene2::CheckGomoku(int row, int col, GomokuSTATUS current)
{
	// ������
	int x, y;

	int count;

	// ���� üũ
	x = row;
	y = col;
	count = 0;

	// ���� ���� ��ġ ã��
	while (gomoku[x - 1][y].status == current && x > 0) x--;

	while (gomoku[x++][y].status == current && x <= 18) count++;

	if (count == 5)
		GameOver(current);

	// ���� üũ
	x = row;
	y = col;
	count = 0;

	// ���� ���� ��ġ ã��
	while (gomoku[x][y - 1].status == current && y > 0) y--;

	while (gomoku[x][y++].status == current && y <= 18) count++;

	if (count == 5)
		GameOver(current);

	// �밢�� üũ
	x = row;
	y = col;
	count = 0;

	// �밢�� ���� ��ġ ã��
	while (gomoku[x - 1][y - 1].status == current
		&& x > 0 && y > 0) {
		x--;
		y--;
	}

	while (gomoku[x++][y++].status == current 
		&& x <= 18 && y <= 18) count++;

	if (count == 5)
		GameOver(current);

	// ���밢�� üũ
	x = row;
	y = col;
	count = 0;

	// ���밢�� ���� ��ġ ã��
	while (gomoku[x - 1][y + 1].status == current
		&& x > 0 && y < 18) {
		x--;
		y++;
	}

	while (gomoku[x++][y--].status == current
		&& x <= 18 && y >= 0) count++;

	if (count == 5)
		GameOver(current);
}

void GameScene2::GameOver(GomokuSTATUS winner)
{
	this->winner = winner;
	isOver = true;
}
