#include "stdafx.h"
#include "MainGame3.h"


MainGame3::MainGame3()
{
}


MainGame3::~MainGame3()
{
}

HRESULT MainGame3::Init()
{
	GameNode2::Init();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			rc[i*4 + j] = RectMake(i * 150 + 100, j * 150 + 100, 100, 100);
	}

	prevTime = GetTickCount();

	number = 0;
	score = 0;
	time = 5000;
	minTime = 5000;
	maxLevel = 1;
	level = 1;
	count = 0;

	brush = CreateSolidBrush(RGB(255, 0, 0));
	clickedBrush = CreateSolidBrush(RGB(0, 255, 0));

	isClicked = false;

	return S_OK;
}

void MainGame3::Release()
{
	GameNode2::Release();
}

void MainGame3::Update()
{
	GameNode2::Update();

	currentTime = GetTickCount();
	if (currentTime - prevTime > time) {

		number = RND->GetInt(16);

		isClicked = false;

		prevTime = GetTickCount();

		count++;
		if (count % 10 == 0) {
			level--;
			count = 0;
			// level ������ time�� ������
			if (time < 1000)
				time += 100;
			else if (time < 5000)
				time += 1000;
		}
	}

}

void MainGame3::Render(HDC hdc)
{
	GameNode2::Render(hdc);

	char str[128];
	
	for (int i = 0; i < 16; i++) {
		if (number == i) {
			// GetStockObject �̸� �������ִ°�
			//FillRect(hdc, &rc[i], (HBRUSH)GetStockObject(BLACK_BRUSH));
			if (!isClicked)
				FillRect(hdc, &rc[i], brush);
			else
				FillRect(hdc, &rc[i], clickedBrush);
		}
		else
			RectangleMake(hdc, rc[i]);
	}



	sprintf_s(str, "���� : %d", level);
	TextOut(hdc, 750, 250, str, strlen(str));

	sprintf_s(str, "���ѽð� : %d ms", time);
	TextOut(hdc, 750, 300, str, strlen(str));

	sprintf_s(str, "���� ���� ��ȸ : %d", 10 - count);
	TextOut(hdc, 750, 350, str, strlen(str));

	sprintf_s(str, "�ְ� ���� : %d", maxLevel);
	TextOut(hdc, 750, 400, str, strlen(str));

	sprintf_s(str, "�ִ� �����ӵ� : %d ms", minTime);
	TextOut(hdc, 750, 450, str, strlen(str));

	//// test
	//sprintf_s(str, "���� ���� : %d", number);
	//TextOut(hdc, 0, 0, str, strlen(str));

}

LRESULT MainGame3::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT crt;
	HDC hMemDC;
	HBITMAP hBitmap, OldBitmap;

	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_TIMER:
		this->Update();
		break;
	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		//this->Render(hdc);
		//EndPaint(hWnd, &ps);

		// ���� ���۸�
		GetClientRect(hWnd, &crt);
		hdc = BeginPaint(hWnd, &ps);

		hMemDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		FillRect(hMemDC, &crt, (HBRUSH)GetStockObject(WHITE_BRUSH));
		this->Render(hMemDC);

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);

		DeleteObject(SelectObject(hMemDC, OldBitmap));
		DeleteDC(hMemDC);

		EndPaint(hWnd, &ps);

		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if (PtInRect(&rc[number], g_ptMouse)) {
			if (!isClicked) {
				isClicked = true;
				score += level * 100;

				// ���� ���� ���� level ���
				if (score >= level * 200) {
					count = 0;
					level++;

					// level ������ time�� ������
					if (time > 1000)
						time -= 1000;
					else if (time > 100)
						time -= 100;
					else if (time > 10)
						time -= 10;

					// �ְ��� �� �ּҽð� ����
					if (maxLevel < level)
						maxLevel = level;
					if (minTime > time)
						minTime = time;
				}

				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
