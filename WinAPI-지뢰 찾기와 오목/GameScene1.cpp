#include "stdafx.h"
#include "GameScene1.h"


GameScene1::GameScene1()
{
}


GameScene1::~GameScene1()
{
}

HRESULT GameScene1::Init()
{
	isDebug = false;

	board = new Image;
	board->Init("images/minesweeper/board.bmp", BOARDSIZEX, BOARDSIZEY, true);

	for (int i = 0; i < 6; i++) {
		number[i] = new Image;
		number[i]->Init("images/minesweeper/number.bmp", 350, 55, 10, 1, true);
	}

	// �����ǿ� ���ڸ� �ֱ� ���� �Լ�
	ChangeNumber(true, MINECOUNT);
	// ��� ������ �� �� �ִ� ����
	flagCount = 0;
	ChangeNumber(false, MINECOUNT - flagCount);

	sizeX = FIELDX / ROW;
	sizeY = FIELDY / COL;

	// �޴�â �̹��� �б�
	menu = new Image;
	menu->Init("images/minesweeper/menu.bmp", BOARDSIZEX, 50);

	// ��� �̹��� �б�
	result = new Image;
	result->Init("images/minesweeper/result.bmp", 200, 68, 4, 1, true);

	// ���� �̹��� �б�
	for (int i = 0; i < 13; i++) {
		mine[i] = new Image;
		// ����
		if (i < 8) {
			sprintf_s(str, "images/minesweeper/%d.bmp", i + 1);
		}
		else {
			switch (i) {
			case 8:
				sprintf_s(str, "images/minesweeper/boom.bmp");
				break;
			case 9:
				sprintf_s(str, "images/minesweeper/empty.bmp");
				break;
			case 10:
				sprintf_s(str, "images/minesweeper/flag.bmp");
				break;
			case 11:
				sprintf_s(str, "images/minesweeper/mine.bmp");
				break;
			case 12:
				sprintf_s(str, "images/minesweeper/ncb.bmp");
				break;
			}
		}
		mine[i]->Init(str, sizeX, sizeY);
	}

	// ó�� 10���� ���� �־�ΰ� ����
	for (int i = 0; i < ROW*COL; i++) {
		if (i < MINECOUNT)
			isMine[i] = true;
		else
			isMine[i] = false;
	}
	// ���� ����
	MineShuffle();

	// ���� ���� �ʱ�ȭ
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (isMine[i*COL + j]) {
				// test
				//mineInfo[i][j].status = STATUS_MINE;
				mineInfo[i][j].status = STATUS_NCB;
				mineInfo[i][j].isMine = true;
			}
			else
			{
				mineInfo[i][j].isMine = false;
				mineInfo[i][j].status = STATUS_NCB;
			}
			mineInfo[i][j].x = STARTX + sizeX * j;
			mineInfo[i][j].y = STARTY + sizeY * i;
			mineInfo[i][j].isFlag = false;
			mineInfo[i][j].nearMineCount = 0;
			mineInfo[i][j].rc = RectMake(
				mineInfo[i][j].x, mineInfo[i][j].y,
				sizeX, sizeY);
		}
	}
	
	isOver = false;
	isClear = false;

	// �ֺ� ���� ���� üũ
	MineCheck();
	// test
	/*for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			switch (mineInfo[i][j].nearMineCount) {
			case 0:
				if(!mineInfo[i][j].isMine)
					mineInfo[i][j].status = STATUS_EMPTY;
				break;
			case 1:
				mineInfo[i][j].status = STATUS_ONE;
				break;
			case 2:
				mineInfo[i][j].status = STATUS_TWO;
				break;
			case 3:
				mineInfo[i][j].status = STATUS_THREE;
				break;
			case 4:
				mineInfo[i][j].status = STATUS_FOUR;
				break;
			}
		}
	}*/

	rcMineExit = RectMake(WINSIZEX / 2 + BOARDSIZEX - 17, 50, 17, 25);
	rcMineRestart = RectMake(678, 151, 50, 68);

	return S_OK;
}

void GameScene1::Release()
{
	SAFE_DELETE(board);
	for (int i = 0; i < 6; i++)
		SAFE_DELETE(number[i]);
	for (int i = 0; i < 13; i++)
		SAFE_DELETE(mine[i]);
	SAFE_DELETE(result);
	SAFE_DELETE(menu);
}

void GameScene1::Update()
{
	// ���콺 Ŭ�� ��
	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		// ���� â ������ �� ����
		if (PtInRect(&rcMineExit, g_ptMouse)) {
			SCENE->ChangeScene("None");
			return;
		}

		// ����� ��ư ������ ���� ����
		if (PtInRect(&rcMineRestart, g_ptMouse)) {
			this->Init();
			return;
		}

		// ���� ã��
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++) {
				if (PtInRect(&mineInfo[i][j].rc, g_ptMouse)
					&& mineInfo[i][j].isFlag == false) {
					// ���� ã�� ������ ��� �ִϸ��̼� ��ȭ
					result->SetFrameX(1);
					if (mineInfo[i][j].isMine) {
						mineInfo[i][j].status = STATUS_BOOM;
						GameOver();
					}
					else
						FindMine(i, j);
				}
			}
		}
	}

	if (isOver || isClear) return;

	// ���콺 ���� �����·� �ִϸ��̼� ��ȭ
	if (INPUT->GetKeyUp(VK_LBUTTON)) {
		result->SetFrameX(0);
	}

	// ��� �ű�
	if (INPUT->GetKeyDown(VK_RBUTTON)) {
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (PtInRect(&mineInfo[i][j].rc, g_ptMouse)) {
					// Ŭ������ ���� ���°� ��� ���� �ʰ����� ������ ��߲ű�
					if (mineInfo[i][j].status == STATUS_NCB
						&& flagCount < MINECOUNT) {
						mineInfo[i][j].isFlag = true;
						mineInfo[i][j].status = STATUS_FLAG;
						flagCount++;
						ChangeNumber(false, MINECOUNT - flagCount);
					}
					else if (mineInfo[i][j].status == STATUS_FLAG) {
						mineInfo[i][j].isFlag = false;
						mineInfo[i][j].status = STATUS_NCB;
						flagCount--;
						ChangeNumber(false, MINECOUNT - flagCount);
					}
				}
			}
		}
	}

	// ���� Ŭ��� ���� üũ
	// Ŭ������ ���� Ÿ���� ������ Ŭ����
	isClear = true;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (!mineInfo[i][j].isMine &&
				mineInfo[i][j].status == STATUS_NCB)
				isClear = false;
		}
	}

	//test
	//if (INPUT->GetKeyDown(VK_SPACE)) {
	//	MineCheck();
	//	for (int i = 0; i < ROW; i++) {
	//		for (int j = 0; j < COL; j++) {
	//			switch (mineInfo[i][j].nearMineCount) {
	//			case 0:
	//				if (!mineInfo[i][j].isMine)
	//					mineInfo[i][j].status = STATUS_EMPTY;
	//				break;
	//			case 1:
	//				mineInfo[i][j].status = STATUS_ONE;
	//				break;
	//			case 2:
	//				mineInfo[i][j].status = STATUS_TWO;
	//				break;
	//			case 3:
	//				mineInfo[i][j].status = STATUS_THREE;
	//				break;
	//			case 4:
	//				mineInfo[i][j].status = STATUS_FOUR;
	//				break;
	//			}
	//		}
	//	}
	//}

	if (isClear) {
		GameClear();
	}

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void GameScene1::Render()
{
	// ���� �׸���
	board->Render(GetMemDC(), WINSIZEX/2, 100);

	// �޴� �׸���
	menu->Render(GetMemDC() , WINSIZEX/2, 100 - menu->GetHeight());

	// ��� �׸���
	result->FrameRender(GetMemDC(), 678, 151);

	// ���� �׸���
	//number->Render(GetMemDC());
	for (int i = 0; i < 6; i++) {
		// ȭ�� ���� ���� ���� ����
		if(i < 3)
		number[i]->FrameRender(GetMemDC(), 
			550 + i * (number[i]->GetFrameWidth() + 5), 157);
		// ȭ�� ������ ���� ���� - ��� ���� ����
		else
		number[i]->FrameRender(GetMemDC(),
			742 + (i-3) * (number[i]->GetFrameWidth() + 5), 157);
	}

	// ���� �׸���

	// test
	//for (int i = 0; i < 9; i++) {
	//	mine[i]->Render(GetMemDC(), 200, 200 + 50*i);
	//}

	// ���¿� ���� ���� �׸���
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			mine[mineInfo[i][j].status]->Render(GetMemDC(),
				mineInfo[i][j].x, mineInfo[i][j].y);
		}
	}
	//// test
	//for (int i = 0; i < ROW; i++) {
	//	for (int j = 0; j < COL; j++)
	//		RectangleMake(GetMemDC(), mineInfo[i][j].rc);
	//}

	//==================   Debug   ====================
	if (isDebug)
	{
		for (int i = 0; i < ROW; i++) {
			for (int j = 0; j < COL; j++) {
				if (mineInfo[i][j].isMine) {
					RectangleMake(GetMemDC(), mineInfo[i][j].rc);
				}
			}
		}

		RectangleMake(GetMemDC(), rcMineExit);
		RectangleMake(GetMemDC(), rcMineRestart);

		// test
		/*for (int i = 0; i < 13; i++)
			mine[i]->Render(GetMemDC(), i*sizeX, 0);*/
	}
	//=================================================
}

void GameScene1::ChangeNumber(bool mine, int number)
{
	if (mine) {
		// ���� �ڸ�
		this->number[0]->SetFrameX(number / 100);
		// ���� �ڸ�
		this->number[1]->SetFrameX(number / 10 % 10);
		// ���� �ڸ�
		this->number[2]->SetFrameX(number % 10);
	}
	else {
		this->number[3]->SetFrameX(number / 100);
		this->number[4]->SetFrameX(number / 10 % 10);
		this->number[5]->SetFrameX(number % 10);
	}
}

void GameScene1::MineShuffle()
{
	for (int i = 0; i < 1000; i++) {
		int sour = RND->GetInt(ROW*COL);
		int dest = RND->GetInt(ROW*COL);

		bool temp = isMine[dest];
		isMine[dest] = isMine[sour];
		isMine[sour] = temp;
	}
}

void GameScene1::MineCheck()
{
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			// �ش� Ÿ���� ���ڸ� ����
			if (mineInfo[i][j].isMine) continue;
			// �ֺ��� ��ġ�� ���� ������ �ֺ� ���� ���� 1 ����
			// ��
			if (i > 0 && mineInfo[i-1][j].isMine) 
				mineInfo[i][j].nearMineCount++;
			// ��
			if (j > 0 && mineInfo[i][j - 1].isMine) 
				mineInfo[i][j].nearMineCount++;
			// �Ʒ�
			if (i < ROW - 1 && mineInfo[i + 1][j].isMine) 
				mineInfo[i][j].nearMineCount++;
			// ����
			if (j < COL - 1 && mineInfo[i][j + 1].isMine) 
				mineInfo[i][j].nearMineCount++;
			// �� ��
			if (j > 0 && i > 0 && mineInfo[i - 1][j - 1].isMine)
				mineInfo[i][j].nearMineCount++;
			// �� �Ʒ�
			if (j > 0 && i < ROW - 1 && mineInfo[i + 1][j - 1].isMine)
				mineInfo[i][j].nearMineCount++;
			// ���� ��
			if (i > 0 && j < COL - 1 && mineInfo[i - 1][j + 1].isMine)
				mineInfo[i][j].nearMineCount++;
			// ���� �Ʒ�
			if (i < ROW - 1 && j < COL - 1 && mineInfo[i + 1][j + 1].isMine)
				mineInfo[i][j].nearMineCount++;
		}
	}
}

void GameScene1::GameOver()
{
	result->SetFrameX(2);

	// Ŭ������ ���� ���� �� �����ֱ�
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (mineInfo[i][j].isMine && mineInfo[i][j].status == STATUS_NCB)
				mineInfo[i][j].status = STATUS_MINE;
		}
	}

	//KillTimer(g_hWnd, 1);
	isOver = true;
}

void GameScene1::GameClear()
{
	result->SetFrameX(3);
	
	//KillTimer(g_hWnd, 1);
	isClear = true;
}

// ����Լ��� ���� ã��
void GameScene1::FindMine(int row, int col)
{
	// �ش� ��ġ�� �����̰ų� Ŭ������ ���� Ÿ���� �ƴϸ� ����(��ȯ)
	if (mineInfo[row][col].isMine ||
		mineInfo[row][col].status != STATUS_NCB) {
		return;
	}
	else {
		// �ֺ� ���� ������ ���� Ÿ�� ���� (Ÿ���� ���� ����)
		switch (mineInfo[row][col].nearMineCount)
		{
		case 0:
			mineInfo[row][col].status = STATUS_EMPTY;
			break;
		case 1:
			mineInfo[row][col].status = STATUS_ONE;
			break;
		case 2:
			mineInfo[row][col].status = STATUS_TWO;
			break;
		case 3:
			mineInfo[row][col].status = STATUS_THREE;
			break;
		case 4:
			mineInfo[row][col].status = STATUS_FOUR;
			break;
		case 5:
			mineInfo[row][col].status = STATUS_FIVE;
			break;
		case 6:
			mineInfo[row][col].status = STATUS_SIX;
			break;
		case 7:
			mineInfo[row][col].status = STATUS_SEVEN;
			break;
		case 8:
			mineInfo[row][col].status = STATUS_EIGHT;
			break;
		}
	}

	// �������� ��� ȣ��
	if (col > 0) FindMine(row, col - 1);
	// ���������� ��� ȣ��
	if (col < COL - 1) FindMine(row, col + 1);
	// �������� ��� ȣ��
	if (row > 0) FindMine(row - 1, col);
	// �Ʒ������� ��� ȣ��
	if (row < ROW - 1) FindMine(row + 1, col);
	// �� �������� ��� ȣ��
	if (row > 0 && col > 0) FindMine(row - 1, col - 1);
	// �� �Ʒ������� ��� ȣ��
	if (row < ROW - 1 && col > 0) FindMine(row + 1, col - 1);
	// ���� �������� ��� ȣ��
	if (row > 0 && col < COL -1) FindMine(row - 1, col + 1);
	// ���� �Ʒ������� ��� ȣ��
	if (row < ROW -1 && col < COL - 1) FindMine(row + 1, col + 1);
}

