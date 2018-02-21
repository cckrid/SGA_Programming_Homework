#include "stdafx.h"
#include "MainGame9.h"


MainGame9::MainGame9()
{
}


MainGame9::~MainGame9()
{
}

void MainGame9::TagInit()
{
	ball.x = WINSIZEX / 2;
	ball.y = WINSIZEY / 2;
	ball.speed = 0;
	ball.width = 50;
	ball.height = 50;
	ball.rc = RectMakeCenter(ball.x, ball.y, ball.width, ball.height);

	player[0].x = WINSIZEX / 2 - 300;
	player[0].y = WINSIZEY / 2;
	player[0].speed = 0;
	player[0].width = 50;
	player[0].height = 50;
	player[0].rc = RectMakeCenter(
		player[0].x, player[0].y, player[0].width, player[0].height);

	player[1].x = WINSIZEX / 2 + 300;
	player[1].y = WINSIZEY / 2;
	player[1].speed = 0;
	player[1].width = 50;
	player[1].height = 50;
	player[1].rc = RectMakeCenter(
		player[1].x, player[1].y, player[1].width, player[1].height);
}

HRESULT MainGame9::Init()
{
	GameNode::Init();

	field = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 750, 600);

	TagInit();

	score[0].x = field.left - 25;
	score[0].y = field.top + (field.bottom - field.top) / 2;
	score[0].score = 0;
	score[0].rc = RectMakeCenter(score[0].x, score[0].y, 50, 200);

	score[1].x = field.right + 25;
	score[1].y = field.top + (field.bottom - field.top) / 2;
	score[1].score = 0;
	score[1].rc = RectMakeCenter(score[1].x, score[1].y, 50, 200);

	power = 0.05f;

	prevTime = GetTickCount();

	isMove = false;

	angle = 0;

	speedLimit = 30;

	POINT old_ptMouse = g_ptMouse;

	return S_OK;
}

void MainGame9::Release()
{
	GameNode::Release();
}

void MainGame9::Update()
{
	GameNode::Update();

	// player0 Control
	// ���콺 ��ư Ŭ�� ������ ������
	//if (INPUT->GetKey(VK_LBUTTON)) {
	//	if (g_ptMouse.x < field.left)
	//		g_ptMouse.x = field.left;
	//	if (g_ptMouse.x > field.left + (field.right - field.left) / 2) {
	//		g_ptMouse.x = field.left + (field.right - field.left) / 2;
	//	}
	//	if (g_ptMouse.y < field.top)
	//		g_ptMouse.y = field.top;
	//	if (g_ptMouse.y > field.bottom)
	//		g_ptMouse.y = field.bottom;

	//	if (PtInRect(&player[0].rc, g_ptMouse)) {

	//		player[0].speed += power;
	//		player[0].x = g_ptMouse.x;
	//		player[0].y = g_ptMouse.y;
	//		player[0].rc = RectMakeCenter(
	//			player[0].x, player[0].y, player[0].width, player[0].height);
	//	}
	//}

	if (g_ptMouse.x < field.left)
		g_ptMouse.x = field.left;
	if (g_ptMouse.x > field.left + (field.right - field.left) / 2) {
		g_ptMouse.x = field.left + (field.right - field.left) / 2;
	}
	if (g_ptMouse.y < field.top)
		g_ptMouse.y = field.top;
	if (g_ptMouse.y > field.bottom)
		g_ptMouse.y = field.bottom;

	player[0].x = g_ptMouse.x;
	player[0].y = g_ptMouse.y;

	player[0].rc = RectMakeCenter(
	player[0].x, player[0].y, player[0].width, player[0].height);

	currentTime = GetTickCount();
	if (currentTime - prevTime > 100) {

		if (old_ptMouse.x != g_ptMouse.x || old_ptMouse.y != g_ptMouse.y) {
			old_ptMouse = g_ptMouse;
			player[0].speed += power;
		}

		player[1].speed += power;

		// player1(AI) Control

		if (GetDistance(player[1].rc, ball.rc) < 100) {

			float angle = GetAngle(player[1].rc, ball.rc);

			if(GetDistance(player[1].rc, ball.rc) < 30){
				//angle = (float)RND->GetInt(3141592) / 1000000.0f;
				angle += (float)RND->GetFromInto(-1, 1) / 1000.0f;
			}

			player[1].x += cos(angle) * player[1].speed;
			player[1].y += -sin(angle) * player[1].speed;
		}
		else {

			//if (player[1].x > ball.x) {
			//	player[1].x -= player[1].speed;
			//}
			//if (player[1].x < ball.x) {
			//	player[1].x += player[1].speed;
			//}
			if (player[1].y < ball.y) {
				player[1].y += player[1].speed;
			}
			if (player[1].y > ball.y) {
				player[1].y -= player[1].speed;
			}
		}

		if (player[1].x < field.left + (field.right - field.left) / 2)
			player[1].x = field.left + (field.right - field.left) / 2;
		if (player[1].x > field.right) {
			player[1].x = field.right / 2;
		}
		if (player[1].y < field.top)
			player[1].y = field.top;
		if (player[1].y > field.bottom)
			player[1].y = field.bottom;

		player[1].rc = RectMakeCenter(
			player[1].x, player[1].y, player[1].width, player[1].height);

		//ball.speed -= 0.05f;

		if (ball.speed <= 0) {
			ball.speed = 0;
			isMove = false;
		}

		if (circleCollision(player[0].rc, ball.rc) == true) {
			ball.speed += player[0].speed;

			// �ð��� �����°� �ӵ� �����̶� �Ǵ�
			// ������� ������ ����
			if (ball.speed > speedLimit)
				ball.speed = speedLimit;

			player[0].speed = 0;
			angle = GetAngle(player[0].rc, ball.rc);
			isMove = true;
		}

		if (circleCollision(player[1].rc, ball.rc) == true) {
			ball.speed += player[1].speed;

			// �ð��� �����°� �ӵ� �����̶� �Ǵ�
			if (ball.speed > speedLimit)
				ball.speed = speedLimit;

			player[1].speed = 0;
			angle = GetAngle(player[1].rc, ball.rc);
			isMove = true;
		}

		if (isMove) {

			// RectMakeCenter�� ���� ���� ��������� -> �ƴ� 
			ball.x += cos(angle) * ball.speed;
			ball.y += -sin(angle) * ball.speed;

			ball.rc = RectMakeCenter(
				ball.x, ball.y, ball.width, ball.height);

			// ���ھ� �κ�
			RECT temp;
			if (IntersectRect(&temp, &ball.rc, &score[0].rc)) {
				score[1].score += 1;
				TagInit();
			}

			if (IntersectRect(&temp, &ball.rc, &score[1].rc)) {
				score[0].score += 1;
				TagInit();
			}

			// ���� ã�� borderCollision���� x,y������ ���ϹǷ� 
			// x, y���� ���� ���־����
			// �Ϻ��� �ذ� ����� �ƴѰ� ����� �� �ӵ� ���ѵ� �ɾ������
			if (borderCollision()) {
				if (ball.rc.left < field.left) {
					ball.rc.left = field.left;
					ball.rc.right = field.left + ball.width;
					ball.x = ball.rc.left + ball.width / 2;
					angle = M_PI - angle;
				}
				else if (ball.rc.top < field.top) {
					ball.rc.top = field.top;
					ball.rc.bottom = field.top + ball.height;
					ball.y = ball.rc.top + ball.height / 2;
					angle = 2 * M_PI - angle;
				}
				else if (ball.rc.right > field.right) {
					ball.rc.right = field.right;
					ball.rc.left = field.right - ball.width;
					ball.x = ball.rc.right - ball.width / 2;
					angle = M_PI - angle;
				}
				else if (ball.rc.bottom > field.bottom) {
					ball.rc.bottom = field.bottom;
					ball.rc.top = field.bottom - ball.height;
					ball.y = ball.rc.bottom - ball.height / 2;
					angle = 2 * M_PI - angle;
				}
			}
		}
	}
}

void MainGame9::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	RectangleMake(memDC, field);

	LineMake(memDC, WINSIZEX / 2, WINSIZEY / 2 - 300,
		WINSIZEX / 2, WINSIZEY / 2 + 300);

	char str[128];

	sprintf_s(str, "�÷��̾�1 �ӵ� : %f", player[0].speed);
	TextOut(memDC, 10, 10, str, strlen(str));

	sprintf_s(str, "�÷��̾�2 �ӵ� : %f", player[1].speed);
	TextOut(memDC, 10, 30, str, strlen(str));

	sprintf_s(str, "�� �ӵ� : %f", ball.speed);
	TextOut(memDC, 10, 50, str, strlen(str));

	HFONT hFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
	HFONT OldFont = (HFONT)SelectObject(memDC, hFont);

	sprintf_s(str, "%d", score[0].score);
	TextOut(memDC, WINSIZEX / 2 - 200, WINSIZEY / 2 - 50, str, strlen(str));

	sprintf_s(str, "%d", score[1].score);
	TextOut(memDC, WINSIZEX / 2 + 150, WINSIZEY / 2 - 50, str, strlen(str));

	SelectObject(memDC, OldFont);
	DeleteObject(hFont);

	for (int i = 0; i < 2; i++) {
		EllipseMake(memDC, player[i].rc);
		RectangleMake(memDC, score[i].rc);
	}

	// �̷��� �ϸ� ���� ��ĥ ����
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, myBrush);

	EllipseMake(memDC, ball.rc);

	SelectObject(memDC, oldBrush);
	DeleteObject(myBrush);



	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

bool MainGame9::circleCollision(RECT rc1, RECT rc2)
{
	POINT center1;
	POINT center2;

	RECT rc[2];
	rc[0] = rc1;
	rc[1] = rc2;

	center1.x = rc[0].left + (rc[0].right - rc[0].left) / 2;
	center1.y = rc[0].top + (rc[0].bottom - rc[0].top) / 2;

	center2.x = rc[1].left + (rc[1].right - rc[1].left) / 2;
	center2.y = rc[1].top + (rc[1].bottom - rc[1].top) / 2;

	int deltaX = center2.x - center1.x;
	int deltaY = center2.y - center1.y;

	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	if (player[0].width / 2 + player[1].width / 2 > distance) {
		return true;
	}
	return false;
}

// ���� ���� ������ �ʵ忡�� �ð� ������ �� ����
bool MainGame9::borderCollision()
{
	//// �̹���� �߸��� �Ǳ� �ϴµ� �̰Ŵ� �簢�� �浹 ����
	//for (int i = 0; i < 4; i++) {
	//	switch (i) {
	//		// left collision
	//	case 0:
	//		if (ball.rc.left < field.left)
	//			return true;
	//		break;
	//		// top collision
	//	case 1:
	//		if (ball.rc.top < field.top)
	//			return true;
	//		break;
	//		// right collsion
	//	case 2:
	//		if (ball.rc.right > field.right)
	//			return true;
	//		break;
	//		// bottom collsion
	//	case 3:
	//		if (ball.rc.bottom > field.bottom)
	//			return true;
	//		break;
	//	}
	//}

	int deltaX, deltaY;
	float distance;
	float radius = ball.height / 2;

	// up
	deltaX = 0;
	deltaY = ball.y - field.top;
	distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (distance < radius) {
		return true;
	}
	// down
	deltaX = 0;
	deltaY = field.bottom - ball.y;
	distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (distance < radius) {
		return true;
	}

	// left
	deltaX = ball.x - field.left;
	deltaY = 0;
	distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (distance < radius) {
		return true;
	}

	// right
	deltaX = field.right - ball.x;
	deltaY = 0;
	distance = sqrt(deltaX * deltaX + deltaY * deltaY);
	if (distance < radius) {
		return true;
	}

	return false;
}

float MainGame9::GetAngle(RECT rc1, RECT rc2)
{
	POINT center1;
	POINT center2;

	RECT rc[2];
	rc[0] = rc1;
	rc[1] = rc2;

	center1.x = rc[0].left + (rc[0].right - rc[0].left) / 2;
	center1.y = rc[0].top + (rc[0].bottom - rc[0].top) / 2;

	center2.x = rc[1].left + (rc[1].right - rc[1].left) / 2;
	center2.y = rc[1].top + (rc[1].bottom - rc[1].top) / 2;

	int deltaX = center2.x - center1.x;
	int deltaY = center2.y - center1.y;

	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	float angle = acos(deltaX / distance);

	if (center2.y > center1.y) {
		angle = 2 * M_PI - angle;
		if (angle >= 2 * M_PI) angle -= 2 * M_PI;
	}

	return angle;
}

float MainGame9::GetDistance(RECT rc1, RECT rc2)
{
	POINT center1;
	POINT center2;

	RECT rc[2];
	rc[0] = rc1;
	rc[1] = rc2;

	center1.x = rc[0].left + (rc[0].right - rc[0].left) / 2;
	center1.y = rc[0].top + (rc[0].bottom - rc[0].top) / 2;

	center2.x = rc[1].left + (rc[1].right - rc[1].left) / 2;
	center2.y = rc[1].top + (rc[1].bottom - rc[1].top) / 2;

	int deltaX = center2.x - center1.x;
	int deltaY = center2.y - center1.y;

	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	return distance;
}
