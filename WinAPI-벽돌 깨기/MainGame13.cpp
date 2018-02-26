#include "stdafx.h"
#include "MainGame13.h"


MainGame13::MainGame13()
{
}


MainGame13::~MainGame13()
{
}

HRESULT MainGame13::Init()
{
	GameNode::Init();

	// block
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 12; j++) {
			block[i * 12 + j].x = 100 + 75 * j;
			block[i * 12 + j].y = 100 + 50 * i;
			block[i * 12 + j].rc = RectMakeCenter(
				block[i*12+j].x, block[i*12+j].y, 75, 50);
			block[i * 12 + j].life = 3 - i;
		}
	}
	// bar
	bar.x = WINSIZEX / 2;
	bar.y = WINSIZEY - WINSIZEY / 8;
	bar.rc = RectMakeCenter(bar.x, bar.y, 200, 50);
	// ball
	ball.angle = -90.0f * PI / 180;
	ball.radius = 25;
	ball.speed = 5.0f;
	ball.pt.x = WINSIZEX / 2;
	ball.pt.y = bar.y - WINSIZEY / 4;

	isOver = false;
	isStart = false;

	return S_OK;
}

void MainGame13::Release()
{
	GameNode::Release();
}

void MainGame13::Update()
{
	GameNode::Update();

	// isStart false �� �׳� return �ؼ� �������� �ʵ��� ��
	if (INPUT->GetKey(VK_SPACE)) isStart = true;
	if (!isStart) return;

	// isOver true �� ���� ����
	if (isOver)
		KillTimer(g_hWnd, 1);

	if (INPUT->GetKey(VK_LEFT)) { bar.x -= 5.0f; }
	if (INPUT->GetKey(VK_RIGHT)) { bar.x += 5.0f; }
	bar.rc = RectMakeCenter(bar.x, bar.y, 200, 50);

	// ball move
	ball.pt.x += cosf(ball.angle) * ball.speed;
	// 0.05f �� �߷°� ���� �ᱹ ������ �� �ֵ���
	ball.pt.y += -sinf(ball.angle) * ball.speed + 0.05f;
	
	// ball �� �浹
	// left
	if (ball.pt.x - ball.radius < 0) {
		ball.pt.x = ball.radius;
		ball.angle = PI - ball.angle;
	}
	// right
	if (ball.pt.x + ball.radius > WINSIZEX) {
		ball.pt.x = WINSIZEX - ball.radius;
		ball.angle = PI - ball.angle;
	}
	// top
	if (ball.pt.y - ball.radius < 0) {
		ball.pt.y = ball.radius;
		ball.angle = -ball.angle;
	}
	// bottom
	if (ball.pt.y + ball.radius > WINSIZEY) {
		isOver = true;
	}

	// ball bar �浹
	if (RectInCircle(&bar.rc, ball.pt, ball.radius)) {
		// �� ����� �ƴѵ� �̻��ϰ� ƨ�ܳ��� ���浹ó�� �����ؾ��� ��
		ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);


		// �� ����� �ƴ�
		//// bar ����
		//if (ball.pt.x < bar.rc.left) {
		//	ball.pt.x = bar.rc.left - ball.radius;
		//	ball.angle = PI - ball.angle;
		//}
		//// bar ������
		//if (ball.pt.x > bar.rc.right) {
		//	ball.pt.x = bar.rc.right + ball.radius;
		//	ball.angle = PI - ball.angle;
		//}
		//// bar ����
		//if (ball.pt.y < bar.rc.top) {
		//	ball.pt.y = bar.rc.top - ball.radius;
		//	ball.angle = -ball.angle;
		//}
		//// bar �Ʒ���
		//if (ball.pt.y > bar.rc.bottom) {
		//	ball.pt.y = bar.rc.bottom + ball.radius;
		//	ball.angle = -ball.angle;
		//}
	}

	// ball block �浹
	for (int i = 0; i < BLOCKMAX; i++) {
		// life 0�� ��� ����
		if (block[i].life == 0) continue;

		// �浹��
		if (RectInCircle(&block[i].rc, ball.pt, ball.radius)) {
			// ��� ������ 1����
			block[i].life -= 1;
			// �� ���� ����
			// �ϴ��� �� �������
			ball.angle = GetAngle(block[i].x, block[i].y, ball.pt.x, ball.pt.y);
		}
	}
}

void MainGame13::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	for (int i = 0; i < BLOCKMAX; i++) {
		if (block[i].life == 0) continue;
		HBRUSH brush = NULL;
		HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, brush);
		switch (block[i].life) {
		case 1:
			brush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 2:
			brush = CreateSolidBrush(RGB(255, 255, 0));
			break;
		case 3:
			brush = CreateSolidBrush(RGB(0, 0, 255));
			break;
		case 4:
			brush = (HBRUSH)GetStockObject(GRAY_BRUSH);
			break;
		}
		RectangleMake(memDC,
			block[i].rc.left - 1,
			block[i].rc.top - 1,
			block[i].rc.right + 1,
			block[i].rc.bottom + 1);
		FillRect(memDC, &block[i].rc, brush);
		SelectObject(memDC, oldBrush);
		DeleteObject(brush);
	}

	RectangleMake(memDC, bar.rc);

	EllipseMakeCenter(memDC, ball.pt.x, ball.pt.y, ball.radius);

	if (isOver) {
		HFONT font = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
		HFONT OldFont = (HFONT)SelectObject(memDC, font);
		
		TextOut(memDC, WINSIZEX / 2 - 150,  WINSIZEY / 2, "Game Over", strlen("Game Over"));

		SelectObject(memDC, OldFont);
		DeleteObject(font);
	}

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

bool MainGame13::RectInCircle(RECT * rect, POINT circleCenter, float radius)
{
	if (rect->left <= circleCenter.x && circleCenter.x <= rect->right
		|| rect->top <= circleCenter.y && circleCenter.y <= rect->bottom) {
		RECT rcEx = {
			rect->left - radius,
			rect->top - radius,
			rect->right + radius,
			rect->bottom + radius
		};

		return PtInRect(&rcEx, circleCenter);
	}
	// �𼭸�
	else {
		float distance[4];
		// left top
		distance[0] = GetDistance(circleCenter.x, circleCenter.y,
			rect->left, rect->top);
		// right top
		distance[1] = GetDistance(circleCenter.x, circleCenter.y,
			rect->right, rect->top);
		// left bottom
		distance[2] = GetDistance(circleCenter.x, circleCenter.y,
			rect->left, rect->bottom);
		distance[3] = GetDistance(circleCenter.x, circleCenter.y,
			rect->right, rect->bottom);
		for (int i = 0; i < 4; i++) {
			if (distance[i] < radius)
				return true;
		}
	}

	return false;
}
