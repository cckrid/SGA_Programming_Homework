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
			block[i * 12 + j].life = 4 - i;
			block[i * 12 + j].isItem = false;
		}
	}
	// bar
	bar.x = WINSIZEX / 2;
	bar.y = WINSIZEY - WINSIZEY / 8;
	bar.width = 200;
	bar.height = 50;
	bar.speed = 15.0f;
	bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);
	
	// ball
	ballCount = 1;
	initSpeed = 5.0f;

	for (int i = 0; i < BALLMAX; i++) {
		ball[i].radius = 15;
		ball[i].speed = 0;
		ball[i].pt.x = WINSIZEX / 2 +
			RND->GetFromInto(-(bar.rc.right - bar.rc.left) / 2, (bar.rc.right - bar.rc.left) / 2);
		ball[i].pt.y = bar.y - (bar.rc.bottom - bar.rc.top) / 2 - ball[i].radius;
		//ball.angle = 45.0f * PI / 180;
		ball[i].angle = GetAngle(bar.x, bar.y, ball[i].pt.x, ball[i].pt.y);
		// ó������ fire�� �ؼ� �߻� �����ϰ�
		ball[i].isFire = (i == 0) ? true : false;
	}
	isOver = false;

	// item
	// ��� �ε����� ���� ���� ������ ��� ��ġ�� ����� ���� 
	int num[BLOCKMAX];
	for (int i = 0; i < BLOCKMAX; i++) {
		num[i] = i;
	}
	//// �׽�Ʈ�� ����
	//int num[12];
	//for (int i = 0; i < 12; i++) {
	//	num[i] = i;
	//}
	// ����
	for (int i = 0; i < 1000; i++) {
		int dest = RND->GetInt(BLOCKMAX);
		int sour = RND->GetInt(BLOCKMAX);
		// �׽�Ʈ
		//int dest = RND->GetInt(BLOCKMAX / 4);
		//int sour = RND->GetInt(BLOCKMAX / 4);
		int temp = num[dest];
		num[dest] = num[sour];
		num[sour] = temp;
	}

	for (int i = 0; i < ITEMMAX; i++) {
		item[i].isFind = false;
		// 7�����̹Ƿ� 1~7 ������ ������ �ֱ� -> 1~6����
		// bullet �� �ϴ� �̱��� �ϱ�� ����
		item[i].kind = (ItemKind)(RND->GetInt(6) + 1);
		// ������ �׽�Ʈ��
		item[i].kind = ITEM_MULT;
		// ������ ����� ��ġ�� ������ �ֱ�
		item[i].x = block[num[i]].x;
		item[i].y = block[num[i]].y;
		item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);
		// �׽�Ʈ��
		//item[i].kind = (ItemKind)(i % 6 + 1);
		//item[i].x = block[3 * 12 + num[i]].x;
		//item[i].y = block[3 * 12 + num[i]].y;
		//item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);
		//block[3 * 12 + num[i]].isItem = true;
		// �������� �� ��Ͽ� ������ ǥ��
		block[num[i]].isItem = true;
	}

	return S_OK;
}

void MainGame13::Release()
{
	GameNode::Release();
}

void MainGame13::Update()
{
	GameNode::Update();


	// isOver true �� ���� ����
	if (isOver)
		KillTimer(g_hWnd, 1);

	if (INPUT->GetKey(VK_LEFT)) { 
		bar.x -= bar.speed; 
		// ���� �� ���
		int width = bar.rc.right - bar.rc.left;
		if (bar.x - width / 2 < 0) {
			bar.x = width / 2;
		}

		for (int i = 0; i < BALLMAX; i++) {
			if (ball[i].isFire == false) continue;
			// ���� �߻���� �ʾ��� �� (���ǵ尡 0�϶�)
			if (ball[i].speed == 0) {
				// ���� �ٿ� ���� ������
				ball[i].pt.x -= bar.speed + 2;
				// �߻� ���� �ʾ����� �ٿ��� ���������� �ʰ�
				if (ball[i].pt.x < bar.rc.left) {
					ball[i].pt.x = bar.rc.left;
				}
				// �����ӿ� ���� �߻� ���� ����
				ball[i].angle = GetAngle(bar.x, bar.y, ball[i].pt.x, ball[i].pt.y);
			}
		}
	}
	if (INPUT->GetKey(VK_RIGHT)) { 
		bar.x += bar.speed;
		// ������ �� ���
		int width = bar.rc.right - bar.rc.left;
		if (bar.x + width / 2 > WINSIZEX) {
			bar.x = WINSIZEX - width / 2;
		}

		for (int i = 0; i < BALLMAX; i++) {
			if (ball[i].isFire == false) continue;
			// ���� �߻���� �ʾ��� �� (���ǵ尡 0�϶�)
			if (ball[i].speed == 0) {
				// ���� �ٿ� ���� ������
				ball[i].pt.x += bar.speed + 2;
				// �߻� ���� �ʾ����� �ٿ��� ���������� �ʰ�
				if (ball[i].pt.x > bar.rc.right) {
					ball[i].pt.x = bar.rc.right;
				}
				// �����ӿ� ���� �߻� ���� ����
				ball[i].angle = GetAngle(bar.x, bar.y, ball[i].pt.x, ball[i].pt.y);
			}
		}
	}
	bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);

	if (INPUT->GetKeyDown(VK_SPACE)) {
		for (int i = 0; i < BALLMAX; i++) {
			if (ball[i].isFire == false) continue;
			ball[i].speed = initSpeed;
		}
	}

	// item move
	for (int i = 0; i < ITEMMAX; i++) {
		// �߰ߵ��� �ʾ��� ����
		if (item[i].isFind == false) continue;
		item[i].y += 3.0f;
		item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);

		// �ٴ����� ������ �������� 
		if (item[i].y + (item[i].rc.bottom - item[i].rc.top) / 2 > WINSIZEY) {
			item[i].isFind = false;
			item[i].y = WINSIZEY + item[i].rc.bottom - item[i].rc.top;
			item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);
		}

		// �ٿ� �浹�� ������ ȿ�� ����
		RECT temp;
		if (IntersectRect(&temp, &item[i].rc, &bar.rc)) {
			switch (item[i].kind)
			{
			case ITEM_MULT:
				ballCount++;
				if (ballCount > BALLMAX)
					ballCount = BALLMAX;
				else {
					int index = ballCount - 1;
					ball[index].isFire = true;
					ball[index].pt.x = ball[index - 1].pt.x + 
						ball[index-1].radius * 2;
					ball[index].pt.y = ball[index - 1].pt.y +
						ball[index-1].radius * 2;
					ball[index].angle =
						GetAngle(ball[index - 1].pt.x, ball[index - 1].pt.y,
							ball[index].pt.x, ball[index].pt.y);
					ball[index].speed = ball[index - 1].speed;
				}
				break;
			case ITEM_FAST:
				for (int i = 0; i < BALLMAX; i++) {
					if (ball[i].isFire == false) continue;
					ball[i].speed += 3.0f;
				}
				break;
			case ITEM_SLOW:
				for (int i = 0; i < BALLMAX; i++) {
					if (ball[i].isFire == false) continue;
					ball[i].speed -= 3.0f;
					if (ball[i].speed <= 1)
						ball[i].speed = 1;
				}
				break;
			case ITEM_LONG:
				bar.width += 30;
				bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);
				break;
			case ITEM_SHORT:
				bar.width -= 30;
				bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);
				break;
			case ITEM_CATCH:
				for (int i = 0; i < BALLMAX; i++) {
					if (ball[i].isFire == false) continue;
					initSpeed = ball[i].speed;
					ball[i].speed = 0;
					ball[i].pt.x = bar.x +
						RND->GetFromInto(-(bar.rc.right - bar.rc.left) / 2, (bar.rc.right - bar.rc.left) / 2);
					ball[i].pt.y = bar.y - (bar.rc.bottom - bar.rc.top) / 2 - ball[i].radius;
				}
				break;
			case ITEM_BULLET:
				break;
			}

			// ȿ�� ����� �ٽ� �߰ߵ��� ���������ؼ� ���� �� �˻翡�� ����
			//item[i].isFind = false;
			// �׳� ���� ���� ����߸����� �ϱ�
			item[i].y += WINSIZEY + item[i].y;
			item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);
		}
	}

	for (int i = 0; i < BALLMAX; i++) {
		if (ball[i].isFire == false) continue;

		// ball move
		ball[i].pt.x += cosf(ball[i].angle) * ball[i].speed;
		// 0.05f �� �߷°� ���� �ᱹ ������ �� �ֵ��� -> �ʿ� ������
		ball[i].pt.y += -sinf(ball[i].angle) * ball[i].speed;
	
		// ball �� �浹
		// left
		if (ball[i].pt.x - ball[i].radius < 0) {
			ball[i].pt.x = ball[i].radius;
			ball[i].angle = PI - ball[i].angle;
		}
		// right
		if (ball[i].pt.x + ball[i].radius > WINSIZEX) {
			ball[i].pt.x = WINSIZEX - ball[i].radius;
			ball[i].angle = PI - ball[i].angle;
		}
		// top
		if (ball[i].pt.y - ball[i].radius < 0) {
			ball[i].pt.y = ball[i].radius;
			ball[i].angle = -ball[i].angle;
		}
		// bottom
		if (ball[i].pt.y + ball[i].radius > WINSIZEY) {
			//ball.pt.y = WINSIZEY - ball.radius;
			//ball.angle = -ball.angle;
			ball[i].isFire = false;
			ballCount--;
			if (ballCount < 1)
				ballCount = 1;
			isOver = true;
			// ���� ��� �˻��ؼ� ����մ°� ������ isOver false�� 
			for (int j = 0; j < BALLMAX; j++) {
				if (ball[j].isFire == true) 
					isOver = false;
			}
		}

		// ball bar �浹
		if (RectInCircle(&bar.rc, ball[i].pt, ball[i].radius)) {
			// �� ����� �ƴѵ� �̻��ϰ� ƨ�ܳ��� ���浹ó�� �����ؾ��� ��
			// bal bar �浹�� �� ����� �´°� ���⵵�� �׷��� ��Ʈ�� �����ϴ�
			ball[i].angle = GetAngle(bar.x, bar.y, ball[i].pt.x, ball[i].pt.y)
				+ RND->GetFromInto(-0.01,0.01);

			//// �� ����� �׳��� ������ -> ������ -> Ʋ����
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
		for (int j = 0; j < BLOCKMAX; j++) {
			// life 0�� ��� ����
			if (block[j].life == 0) continue;

			// �浹��
			if (RectInCircle(&block[j].rc, ball[i].pt, ball[i].radius)) {
				// ��� ������ 1����
				block[j].life -= 1;
				// ����� �������� ������ ������ �������� 0�� �Ǿ��� ��
				if (block[j].isItem && block[j].life == 0) {
					// �������� ã��
					for (int k = 0; k < ITEMMAX; k++) {
						// �߰ߵ� ������ ����
						if (item[k].isFind == true) continue;

						// ����� ��ġ�� �������� ��ġ�� ���� �� ������ �߰�
						if (block[j].x == item[k].x && block[j].y == item[k].y) {
							item[k].isFind = true;
							break;
						}
					}
				}
				// �� ���� ����
				// �ϴ��� �� �������
				//ball.angle = GetAngle(block[i].x, block[i].y, ball.pt.x, ball.pt.y);
				// block ����
				if (ball[i].pt.x < block[j].rc.left) {
					ball[i].pt.x = block[j].rc.left - ball[i].radius;
					ball[i].angle = PI - ball[i].angle;
				}
				// bar ������
				if (ball[i].pt.x > block[j].rc.right) {
					ball[i].pt.x = block[j].rc.right + ball[i].radius;
					ball[i].angle = PI - ball[i].angle;
				}
				// bar ����
				if (ball[i].pt.y < block[j].rc.top) {
					ball[i].pt.y = block[j].rc.top - ball[i].radius;
					ball[i].angle = -ball[i].angle;
				}
				// bar �Ʒ���
				if (ball[i].pt.y > block[j].rc.bottom) {
					ball[i].pt.y = block[j].rc.bottom + ball[i].radius;
					ball[i].angle = -ball[i].angle;
				}
			}
		}

		// ball ball �浹
		for (int j = 0; j < BALLMAX; j++) {
			if (ball[j].isFire == false) continue;
			if (i == j) continue;

			float length = GetDistance(
				ball[i].pt.x, ball[i].pt.y, ball[j].pt.x, ball[j].pt.y);
			if (length > ball[i].radius + ball[j].radius) continue;

			ball[i].angle = 
				GetAngle(ball[j].pt.x, ball[j].pt.y, ball[i].pt.x, ball[i].pt.y);
			ball[j].angle =
				GetAngle(ball[i].pt.x, ball[i].pt.y, ball[j].pt.x, ball[j].pt.y);
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
			if (block[i].isItem)
				// ���̱��� �߰��� �־��ִ� �Լ�
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0));
			else
				brush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 2:
			if (block[i].isItem)
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 0));
			else
				brush = CreateSolidBrush(RGB(255, 255, 0));
			break;
		case 3:
			if(block[i].isItem)
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 0, 255));
			else
				brush = CreateSolidBrush(RGB(0, 0, 255));
			break;
		case 4:
			if (block[i].isItem)
				brush = CreateHatchBrush(HS_DIAGCROSS, RGB(127, 127, 127));
			else
				brush = CreateSolidBrush(RGB(127, 127, 127));
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

	char str[128];

	sprintf_s(str, "�� ����: %d", ballCount);
	TextOut(memDC, 10, 10, str, strlen(str));

	for (int i = 0; i < ITEMMAX; i++) {
		// �׽�Ʈ ���ؼ� ��� ���̰� �а�
		//if (item[i].isFind == false) continue;
		RectangleMake(memDC, item[i].rc);
		switch (item[i].kind)
		{
		case ITEM_MULT:
			sprintf_s(str, "Multi");
			break;
		case ITEM_FAST:
			sprintf_s(str, "Speed");
			break;
		case ITEM_SLOW:
			sprintf_s(str, "Slow");
			break;
		case ITEM_LONG:
			sprintf_s(str, "Long");
			break;
		case ITEM_SHORT:
			sprintf_s(str, "Short");
			break;
		case ITEM_CATCH:
			sprintf_s(str, "Catch");
			break;
		case ITEM_BULLET:
			sprintf_s(str, "Bullet");
			break;
		}
		TextOut(memDC, item[i].x - (item[i].rc.right - item[i].rc.left)/2 + 5, 
			item[i].y - 5, str, strlen(str));
	}

	RectangleMake(memDC, bar.rc);

	for (int i = 0; i < BALLMAX; i++) {
		if (ball[i].isFire == false) continue;
		EllipseMakeCenter(memDC, ball[i].pt.x, ball[i].pt.y, ball[i].radius);
	}

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
