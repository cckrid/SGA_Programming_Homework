#include "stdafx.h"
#include "MainGame6.h"


MainGame6::MainGame6()
{
}


MainGame6::~MainGame6()
{
}

HRESULT MainGame6::Init()
{
	GameNode5::Init();

	player = RectMake(100, 450, 50, 50);

	rc[0] = RectMake(750, 450, 50, 50);

	for (int i = 1; i < 10; i++) {
		rc[i] = RectMake(
			RND->GetFromInto(1000, 2000),  
			RND->GetFromInto(0, 450),
			50, 50);
	}

	score = 0;

	isStart = false;
	isJump = false;
	isOver = false;

	//gravity = 10;
	//jumpPower = 25;
	//jumpTime = 0;

	jumpPower = 3;
	gravity = 0;

	prevTime = GetTickCount();

	return S_OK;
}

void MainGame6::Release()
{
	GameNode5::Release();
}

void MainGame6::Update()
{
	GameNode5::Update();

	if (INPUT->GetKey(VK_SPACE)) {
		isJump = true;
		gravity += jumpPower;
	}

	currentTime = GetTickCount();
	if (currentTime - prevTime > 50) {

		if (isJump) {
			//// ���� ���� 1 (�߷°��ӵ�) 
			//// �̹�� ������ �ڿ������� ���� �ϰ��ӵ��� �̻���
			//// ��� �߸� ����ѵ�
			//// y = -a*x + b (a �߷°��Ӽ�, b �ʱ� �����ӵ�)
			//// �����Ͽ� y = (-a/2)*x*x + (b*x) ���� (x: �����ð�, y: ������Ʈ ����)
			//int height = 
			//	(jumpTime * jumpTime * -gravity / 2) + (jumpTime*jumpPower);
			//
			//player.top -= height;
			//player.bottom -= height;

			//jumpTime += 1;

			//if (height < 0) {
			//	isJump = false;
			//	jumpTime = 0;
			//	player.top = 450;
			//	player.bottom = 500;
			//}

			// ���� ���� 2 ��α� ���� �� ����� �׳��� �̻���
			// ������ ����ٴϰ� ����
			//if (gravity > -35) {
				gravity -= 2;
				player.top -= gravity;
				player.bottom -= gravity;
				if (player.top > 450) {
					player.top = 450;
					player.bottom = 500;
					gravity = 0;
					isJump = false;
				}
			//}
		}

		/*rc[0].left -= 5.0f;
		rc[0].right -= 5.0f;*/

		for (int i = 0; i < 10; i++) {
			if (rc[i].right < 0) {
				rc[i] = RectMake(
					RND->GetFromInto(1000, 2000),
					RND->GetFromInto(0, 450),
					50, 50);
			}

			rc[i].left -= 5.0f;
			rc[i].right -= 5.0f;
		}

		for (int i = 0; i < 10; i++) {
			RECT temp;
			if (IntersectRect(&temp, &player, &rc[i])) {
				isOver = true;
			}
		}

		score++;

		prevTime = GetTickCount();
	}
}

void MainGame6::Render(HDC hdc)
{
	GameNode5::Render(hdc);

	char str[128];
	
	//FillRect(hdc, &player, (HBRUSH)GetStockObject(BLACK_BRUSH));
	RectangleMake(hdc, player);

	//RectangleMake(hdc, rc[0]);

	for (int i = 0; i < 10; i++) {
		//RectangleMake(hdc, rc[i]);
		FillRect(hdc, &rc[i], (HBRUSH)GetStockObject(BLACK_BRUSH));
	}

	LineMake(hdc, 0, 500, 1000, 500);

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
	OldFont = (HFONT)SelectObject(hdc, hFont);

	sprintf_s(str, "�÷��̾� ���� : %d", -player.top + 450);
	TextOut(hdc, 0, 520, str, strlen(str));

	sprintf_s(str, "���� �Ŀ� : %d", -gravity);
	TextOut(hdc, 0, 570, str, strlen(str));

	sprintf_s(str, "���� : %d", score);
	TextOut(hdc, 0, 620, str, strlen(str));

	if (isOver) {
		TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2 - 150 , "���� ����", strlen("���� ����"));
		this->Release();
	}

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
}
