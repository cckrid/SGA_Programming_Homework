#include "stdafx.h"
#include "MainGame15.h"


MainGame15::MainGame15()
{
}


MainGame15::~MainGame15()
{
}

HRESULT MainGame15::Init()
{
	GameNode::Init();

	// �̹��� ũ�� 540 x 566
	// 180 x 188 �ϳ�
	// �ε� init�� ���� �� ũ��� offset�� ������
	image = new Image;
	image->Init("images/image.bmp", 300, 300, false, RGB(255,0,255));

	// slide init
	SlideInit();

	// shuffle
	for (int i = 0; i < 1000; i++) {
		int sour = RND->GetInt(9);
		int dest = RND->GetInt(9);

		//// �̰� �ȵǴµ� -> �Ǵµ� offset���� ���� �ٲ� x,y �� �ٲ���ߵ�
		// offset�� �ٲ����� x,y�� ����
		//tagSlideInfo temp = slide[sour];
		//slide[sour] = slide[dest];
		//slide[dest] = temp;

		SlideSwap(sour, dest);
	}

	pointer = 8;

	return S_OK;
}

void MainGame15::Release()
{
	GameNode::Release();
	SAFE_DELETE(image);
}

void MainGame15::Update()
{
	GameNode::Update();

	if (INPUT->GetKeyDown(VK_LEFT)) {
		// �� ������ �ƴϸ� ������
		if (pointer % 3 != 0) {
			SlideSwap(pointer, pointer - 1);
			pointer--;
		}
	}
	if (INPUT->GetKeyDown(VK_RIGHT)) {
		// �� �������� �ƴϸ� ������
		if ((pointer + 1) % 3 != 0) {
			SlideSwap(pointer, pointer + 1);
			pointer++;
		}
	}
	if (INPUT->GetKeyDown(VK_UP)) {
		// �� �Ʒ� �ƴϸ�
		if (pointer >= 3) {
			SlideSwap(pointer, pointer - 3);
			pointer -= 3;
		}
	}
	if(INPUT->GetKeyDown(VK_DOWN)) {
		// �� �� �ƴϸ�
		if (pointer < 6) {
			SlideSwap(pointer, pointer + 3);
			pointer += 3;
		}
	}
	if (INPUT->GetKey(VK_SPACE)) { 
		SlideInit(); 
		pointer = 8;
	}
}

void MainGame15::Render(HDC hdc)
{
	HDC memDC = this->GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================

	//image->Render(memDC, 100, 100);
	//image->Render(memDC, 300, 100, 200, 0, 100, 100);

	//int i = 0;
	//int j = 2;

	//image->Render(memDC,
	//	slide[i * 3 + j].x + 100,
	//	slide[i * 3 + j].y + 100,
	//	slide[i * 3 + j].offsetX, slide[i * 3 + j].offsetY,
	//	slide[i * 3 + j].width, slide[i * 3 + j].height);

	// �����̵�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			image->Render(memDC,
				slide[i * 3 + j].x + 100,
				slide[i * 3 + j].y + 100,
				slide[i*3+j].offsetX, slide[i*3+j].offsetY, 
				slide[i*3+j].width, slide[i*3+j].height);
		}
	}

	char str[64];
	sprintf_s(str, "point : %d", pointer);
	TextOut(memDC, 0, 0, str, strlen(str));

	// ����
	image->Render(memDC, 550, 100);

	//=================================================
	this->GetBackBuffer()->Render(hdc);
}

void MainGame15::SlideInit()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			slide[i * 3 + j].width = 100;
			slide[i * 3 + j].height = 100;
			slide[i * 3 + j].offsetX = slide[i * 3 + j].width * j;
			slide[i * 3 + j].offsetY = slide[i * 3 + j].height * i;
			slide[i * 3 + j].x = slide[i * 3 + j].width * j;
			slide[i * 3 + j].y = slide[i * 3 + j].height * i;
		}
	}
}

void MainGame15::SlideSwap(int sour, int dest)
{
	float tempX, tempY;
	tempX = slide[sour].offsetX;
	tempY = slide[sour].offsetY;
	slide[sour].offsetX = slide[dest].offsetX;
	slide[sour].offsetY = slide[dest].offsetY;
	slide[dest].offsetX = tempX;
	slide[dest].offsetY = tempY;
}
