#include "stdafx.h"
#include "GameNode.h"


GameNode::GameNode()
{
}


GameNode::~GameNode()
{
}

HRESULT GameNode::Init()
{
	SetTimer(g_hWnd, 1, 10, NULL);

	return S_OK;
}

void GameNode::Release()
{
	KillTimer(g_hWnd, 1);
	INPUT->ReleaseSingleton();
}

void GameNode::Update()
{
	//InvalidateRect(g_hWnd, NULL, TRUE);
	// ���� ���۸���
	InvalidateRect(g_hWnd, NULL, FALSE);
}

void GameNode::Render(HDC hdc)
{
}

LRESULT GameNode::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT crt; // ���簢�� ��� ���� ����
	HDC hMemDC; // HDC�� �ϳ� �� ����
	HBITMAP hBitmap, OldBitmap;	// HBITMAP�� �뷫 ���� �ǹ�, ���� 2�� ����

	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_TIMER:
		this->Update();
		break;
	case WM_PAINT:
		//// ���� ���
		//hdc = BeginPaint(hWnd, &ps);
		//this->Render(hdc);
		//EndPaint(hWnd, &ps);

		// ���� ���۸� �õ�
		GetClientRect(hWnd, &crt);
		hdc = BeginPaint(hWnd, &ps);

		hMemDC = CreateCompatibleDC(hdc);	// hMemDC�� ���� DC (hdc)�� �°� �� DC ����
		hBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom); // crt �԰ݴ�� ���� ����
		OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); // ���� ��ü

		FillRect(hMemDC, &crt, (HBRUSH)GetStockObject(WHITE_BRUSH)); // ������� ȭ�� �����
		this->Render(hMemDC);

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY); // �貸 �׸���

		DeleteObject(SelectObject(hMemDC, OldBitmap)); // ���� ������� �� �� ����
		DeleteDC(hMemDC); // hMemDC ����

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
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
