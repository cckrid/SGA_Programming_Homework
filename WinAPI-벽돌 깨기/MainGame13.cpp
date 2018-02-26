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
	bar.speed = 5.0f;
	bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);
	// ball
	ball.radius = 15;
	ball.speed = 0;
	ball.pt.x = WINSIZEX / 2 +
		RND->GetFromInto(-(bar.rc.right - bar.rc.left)/2, (bar.rc.right-bar.rc.left)/2);
	ball.pt.y = bar.y - (bar.rc.bottom - bar.rc.top) / 2 - ball.radius;
	//ball.angle = 45.0f * PI / 180;
	ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);

	isOver = false;

	// item
	// ��� �ε����� ���� ���� ������ ��� ��ġ�� ����� ���� 
	int num[BLOCKMAX];
	for (int i = 0; i < BLOCKMAX; i++) {
		num[i] = i;
	}
	// ����
	for (int i = 0; i < 1000; i++) {
		int dest = RND->GetInt(BLOCKMAX);
		int sour = RND->GetInt(BLOCKMAX);
		int temp = num[dest];
		num[dest] = num[sour];
		num[sour] = temp;
	}

	for (int i = 0; i < ITEMMAX; i++) {
		item[i].isFind = false;
		// 7�����̹Ƿ� 1~7 ������ ������ �ֱ� -> 1~6����
		// bullet �� �ϴ� �̱��� �ϱ�� ����
		item[i].kind = (ItemKind)(RND->GetInt(6) + 1);
		//item[i].kind = ITEM_CATCH;
		// ������ ����� ��ġ�� ������ �ֱ�
		item[i].x = block[num[i]].x;
		item[i].y = block[num[i]].y;
		item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);
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

		// ���� �߻���� �ʾ��� �� (���ǵ尡 0�϶�)
		if (ball.speed == 0) {
			// ���� �ٿ� ���� ������
			ball.pt.x -= bar.speed+2;
			// �߻� ���� �ʾ����� �ٿ��� ���������� �ʰ�
			if (ball.pt.x < bar.rc.left) {
				ball.pt.x = bar.rc.left;
			}
			// �����ӿ� ���� �߻� ���� ����
			ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);
		}
	}
	if (INPUT->GetKey(VK_RIGHT)) { 
		bar.x += 5.0f;
		// ������ �� ���
		int width = bar.rc.right - bar.rc.left;
		if (bar.x + width / 2 > WINSIZEX) {
			bar.x = WINSIZEX - width / 2;
		}
		// ���� �߻���� �ʾ��� �� (���ǵ尡 0�϶�)
		if (ball.speed == 0) {
			// ���� �ٿ� ���� ������
			ball.pt.x += bar.speed+2;
			// �߻� ���� �ʾ����� �ٿ��� ���������� �ʰ�
			if (ball.pt.x > bar.rc.right) {
				ball.pt.x = bar.rc.right;
			}
			// �����ӿ� ���� �߻� ���� ����
			ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);
		}
	}
	bar.rc = RectMakeCenter(bar.x, bar.y, bar.width, bar.height);

	if (INPUT->GetKeyDown(VK_SPACE)) {
		ball.speed = 5.0f;
	}

	// item move
	for (int i = 0; i < ITEMMAX; i++) {
		// �߰ߵ��� �ʾ��� ����
		if (item[i].isFind == false) continue;
		item[i].y += 3.0f;
		item[i].rc = RectMakeCenter(item[i].x, item[i].y, 50, 40);

		// �ٴ����� ������ �������� 
		if (item[i].y + (item[i].rc.bottom - item[i].rc.top) / 2 > WINSIZEY)
			item[i].isFind = false;

		// �ٿ� �浹�� ������ ȿ�� ����
		RECT temp;
		if (IntersectRect(&temp, &item[i].rc, &bar.rc)) {
			switch (item[i].kind)
			{
			case ITEM_MULTY:
				break;
			case ITEM_FAST:
				ball.speed += 3.0f;
				break;
			case ITEM_SLOW:
				ball.speed -= 3.0f;
				if (ball.speed <= 1)
					ball.speed = 1;
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
				ball.speed = 0;
				ball.pt.x = bar.x +
					RND->GetFromInto(-(bar.rc.right - bar.rc.left) / 2, (bar.rc.right - bar.rc.left) / 2);
				ball.pt.y = bar.y - (bar.rc.bottom - bar.rc.top) / 2 - ball.radius;
				break;
			case ITEM_BULLET:
				break;
			}

			// ȿ�� ����� �ٽ� �߰ߵ��� ���������ؼ� ���� �� �˻翡�� ����
			item[i].isFind = false;
		}
	}

	// ball move
	ball.pt.x += cosf(ball.angle) * ball.speed;
	// 0.05f �� �߷°� ���� �ᱹ ������ �� �ֵ��� -> �ʿ� ������
	ball.pt.y += -sinf(ball.angle) * ball.speed;
	
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
		//ball.pt.y = WINSIZEY - ball.radius;
		//ball.angle = -ball.angle;
		isOver = true;
	}

	// ball bar �浹
	if (RectInCircle(&bar.rc, ball.pt, ball.radius)) {
		// �� ����� �ƴѵ� �̻��ϰ� ƨ�ܳ��� ���浹ó�� �����ؾ��� ��
		// bal bar �浹�� �� ����� �´°� ���⵵�� �׷��� ��Ʈ�� �����ϴ�
		ball.angle = GetAngle(bar.x, bar.y, ball.pt.x, ball.pt.y);

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
	for (int i = 0; i < BLOCKMAX; i++) {
		// life 0�� ��� ����
		if (block[i].life == 0) continue;

		// �浹��
		if (RectInCircle(&block[i].rc, ball.pt, ball.radius)) {
			// ��� ������ 1����
			block[i].life -= 1;
			// ����� �������� ������ ������ �������� 0�� �Ǿ��� ��
			if (block[i].isItem && block[i].life == 0) {
				// �������� ã��
				for (int j = 0; j < ITEMMAX; j++) {
					// �߰ߵ� ������ ����
					if (item[j].isFind == true) continue;

					// ����� ��ġ�� �������� ��ġ�� ���� �� ������ �߰�
					if (block[i].x == item[j].x && block[i].y == item[j].y) {
						item[j].isFind = true;
						break;
					}
				}
			}
			// �� ���� ����
			// �ϴ��� �� �������
			//ball.angle = GetAngle(block[i].x, block[i].y, ball.pt.x, ball.pt.y);
			// block ����
			if (ball.pt.x < block[i].rc.left) {
				ball.pt.x = block[i].rc.left - ball.radius;
				ball.angle = PI - ball.angle;
			}
			// bar ������
			if (ball.pt.x > block[i].rc.right) {
				ball.pt.x = block[i].rc.right + ball.radius;
				ball.angle = PI - ball.angle;
			}
			// bar ����
			if (ball.pt.y < block[i].rc.top) {
				ball.pt.y = block[i].rc.top - ball.radius;
				ball.angle = -ball.angle;
			}
			// bar �Ʒ���
			if (ball.pt.y > block[i].rc.bottom) {
				ball.pt.y = block[i].rc.bottom + ball.radius;
				ball.angle = -ball.angle;
			}
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

	for (int i = 0; i < ITEMMAX; i++) {
		// �׽�Ʈ ���ؼ� ��� ���̰� �а�
		//if (item[i].isFind == false) continue;
		RectangleMake(memDC, item[i].rc);
		switch (item[i].kind)
		{
		case ITEM_MULTY:
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
