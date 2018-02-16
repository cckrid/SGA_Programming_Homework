#include "stdafx.h"
#include "MainGame2.h"


MainGame2::MainGame2()
{
	Init();
}


MainGame2::~MainGame2()
{
}

void MainGame2::SetupLevelInfo()
{
	lvInfo[0] = { 20, 50, 10.0f };
	lvInfo[1] = { 50, 20, 10.0f };
	lvInfo[2] = { 50, 20, 20.0f };
	lvInfo[3] = { 100, 10, 30.0f };
}

void MainGame2::SetupRectInfo()
{
	if (currentLevel == LEVEL_END) {
		for (int i = 0; i < lvInfo[currentLevel].count; i++) {
			int size = RND->GetFromInto(
				lvInfo[currentLevel].size,
				lvInfo[currentLevel].size + 10);
			rcInfo[i].rect = RectMakeCenter
			(
				RND->GetInt(WINSIZEX),
				RND->GetFromInto(-300, -50),
				size,
				size
			);
			rcInfo[i].speed =
				RND->GetFromInto(
					lvInfo[currentLevel].speed - 10.0f,
					lvInfo[currentLevel].speed + 20.0f);
		}
	}
	else {
		for (int i = 0; i < lvInfo[currentLevel].count; i++) {
			rcInfo[i].rect = RectMakeCenter
			(
				RND->GetInt(WINSIZEX),
				RND->GetFromInto(-500, -50),
				lvInfo[currentLevel].size,
				lvInfo[currentLevel].size
			);
			rcInfo[i].speed =
				RND->GetFromInto(
					lvInfo[currentLevel].speed - 3.0f,
					lvInfo[currentLevel].speed + 3.0f);
		}
	}
}

HRESULT MainGame2::Init()
{
	GameNode::Init();

	SetupLevelInfo();

	currentLevel = LEVEL_ZERO;

	SetupRectInfo();

	player = RectMake(WINSIZEX / 2 - 50, WINSIZEY - 100, 30, 30);

	isLevelUp = true;
	isGameOver = false;

	timeCount = 0;
	prevTime = GetTickCount();

	score = 0;

	return S_OK;
}

void MainGame2::Release()
{
	GameNode::Release();
}

void MainGame2::Update()
{
	GameNode::Update();

	if (isLevelUp) {
		if (INPUT->GetKeyDown(VK_SPACE))
			isLevelUp = false;
		return;
	}

	if (INPUT->GetKey(VK_LEFT)) {
		player.left -= 3.0f;
		player.right -= 3.0f;
	}

	if (INPUT->GetKey(VK_RIGHT)) {
		player.left += 3.0f;
		player.right += 3.0f;
	}

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {

		for (int i = 0; i < lvInfo[currentLevel].count; i++) {
			// rect�� �ٴڿ� ������ ������ ��ġ�� �ٽ� ���� ����������
			if (rcInfo[i].rect.bottom >= WINSIZEY) {
				rcInfo[i].rect = rcInfo[i].rect = RectMakeCenter
				(
					RND->GetInt(WINSIZEX),
					0,
					lvInfo[currentLevel].size,
					lvInfo[currentLevel].size
				);
			}

			rcInfo[i].rect.top += rcInfo[i].speed;
			rcInfo[i].rect.bottom += rcInfo[i].speed;
		}

		timeCount++;

		// 0.1�ʸ��� ���� 10���� ����
		score += 10;

		// 10�ʸ��� Level �ø�
		if (timeCount % 100 == 0) {
			if (currentLevel != LEVEL_END) {
				currentLevel = (LEVEL)(currentLevel + 1);
				SetupRectInfo();
				isLevelUp = true;
			}
		}

		prevTime = GetTickCount();
	}

	for (int i = 0; i < lvInfo[currentLevel].count; i++) {
		RECT temp;
		if (IntersectRect(&temp, &player, &rcInfo[i].rect)) {
			isGameOver = true;
			this->Release();
			break;
		}
	}
}

void MainGame2::Render(HDC hdc)
{
	GameNode::Render(hdc);

	for (int i = 0; i < lvInfo[currentLevel].count; i++) {
		RectangleMake(hdc, rcInfo[i].rect);
		//EllipseMake(hdc, rcInfo[i].rect);
	}

	RectangleMake(hdc, player);

	char str[128];
	sprintf_s(str, "���� : %d", score);
	TextOut(hdc, WINSIZEX/ 2 - 50, 100, str, strlen(str));

	if (isGameOver) {
		TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50, 
			"���� ����", strlen("���� ����"));
	}

	if (isLevelUp) {
		TextOut(hdc, WINSIZEX / 2 - 85, WINSIZEY / 2,
			"�����Ϸ��� Space", strlen("�����Ϸ��� Space"));
		switch (currentLevel)
		{
		case LEVEL_ZERO:
			TextOut(hdc, WINSIZEX / 2 - 85, WINSIZEY / 2 - 50,
				"�� ���ϱ� ����", strlen("�� ���ϱ� ����"));
			break;
		case LEVEL_ONE:
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50,
				"���� ��!", strlen("���� ��!"));
			TextOut(hdc, WINSIZEX / 2 - 85, WINSIZEY / 2 + 50,
				"���� ����, ũ�� ����", strlen("���� ����, ũ�� ����"));
			break;
		case LEVEL_TWO:
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50,
				"���� ��!", strlen("���� ��!"));
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 + 50,
				"�ӵ� ����", strlen("�ӵ� ����"));
			break;
		case LEVEL_END:
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 50,
				"���� ��!", strlen("���� ��!"));
			TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 + 50,
				"�ӵ� ũ�� ����", strlen("�ӵ� ũ�� ����"));
			break;
		default:
			break;
		}
	}
}
