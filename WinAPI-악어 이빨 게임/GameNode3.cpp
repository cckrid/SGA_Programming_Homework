#include "stdafx.h"
#include "GameNode3.h"


GameNode3::GameNode3()
{
}


GameNode3::~GameNode3()
{
}

HRESULT GameNode3::Init()
{
	SetTimer(g_hWnd, 1, 10, NULL);

	return S_OK;
}

void GameNode3::Release()
{
	KillTimer(g_hWnd, 1);
	INPUT->ReleaseSingleton();
}

void GameNode3::Update()
{
	//InvalidateRect(g_hWnd, NULL, TRUE);
	InvalidateRect(g_hWnd, NULL, FALSE);
}

void GameNode3::Render(HDC hdc)
{
}

LRESULT GameNode3::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
