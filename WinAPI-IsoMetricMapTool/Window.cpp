#include "stdafx.h"
#include "Window.h"
#include "GameNode.h"

#include "IsoMap.h"

POINT Window::ptMouse = POINT{ 0,0 };
CTRL Window::_currentCTRL = CTRL_DRAW;

Window::Window()
{
	m_backBuffer = new Image();
	m_backBuffer->Init(SUBWINSIZEX, SUBWINSIZEY);
}


Window::~Window()
{
	SAFE_DELETE(m_backBuffer);
}

void Window::Init()
{
	CreateSubWindow();

	isActive = false;

	int tempX = 10;

	_btnDraw = CreateWindow("button", "Tile",
		// �ڽ����� �����ϸ� ���ʿ� �������
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		// Ŭ�������� � ���� ��ȯ���� hMenu
		tempX, 0, 100, 20, hWnd, HMENU(0), g_hInstance, NULL);
	_btnEraser = CreateWindow("button", "Eraser",
		// �ڽ����� �����ϸ� ���ʿ� �������
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		// Ŭ�������� � ���� ��ȯ���� hMenu
		tempX, 30, 100, 20, hWnd, HMENU(1), g_hInstance, NULL);

	_btnSave = CreateWindow("button", "Save",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 0, 100, 20, hWnd, HMENU(2), g_hInstance, NULL);

	_btnLoad = CreateWindow("button", "Load",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		tempX + 115, 30, 100, 20, hWnd, HMENU(3), g_hInstance, NULL);

	clickFrame = { 0,0 };
}

void Window::Release()
{

}

void Window::Update()
{

	if (currentScene != NULL)
	{
		currentScene->Update();
	}
}

void Window::Render()
{
	HDC hdc = GetDC(hWnd);
	PatBlt(m_backBuffer->GetMemDC(), 0, 0, SUBWINSIZEX, SUBWINSIZEY, WHITENESS);
	//==============================
	if (currentScene != NULL)
	{
		currentScene->Render(m_backBuffer->GetMemDC());
	}
		
	//=============================
	m_backBuffer->Render(hdc);
	ReleaseDC(hWnd, hdc);
}

void Window::SetScene(GameNode * scene)
{
	currentScene = scene;
	currentScene->Init();
}

LRESULT Window::WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_MOUSEMOVE:
		SUBWIN->SetIsActive(true);
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		break;
	case WM_PAINT:
	{
		//HDC hdc = GetDC(hWnd);
		//IMAGE->FindImage("sub_bg")->Render(hdc);
		//ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_COMMAND:
		// ���α׷� ���� �� ����ڰ� �޴� �׸��� �����ϸ� �߻��ϴ� �޼���
		switch (LOWORD(wParam))
		{
		default:
			//// Ŭ������ �� 0,1,2 �� �� �ϳ��� ���� 
			//_currentCTRL = (CTRL)(LOWORD(wParam));
			switch (LOWORD(wParam)) {
			case CTRL_DRAW:
			case CTRL_ERASER:
				_currentCTRL = (CTRL)(LOWORD(wParam));
				break;
			case CTRL_SAVE:
				SUBWIN->GetIsoMap()->Save();
				break;
			case CTRL_LOAD:
				SUBWIN->GetIsoMap()->Load();
				break;
			}
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::CreateSubWindow()
{
	// �α� ������ ����
	int x, y, cx, cy;
	WNDCLASS wc;
	RECT rc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)Window::WndLogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "sub";

	RegisterClass(&wc);

	// �θ� ������ �����ʿ� ��ġ�ϰ�
	RECT rcWin;
	GetWindowRect(g_hWnd, &rcWin);

	cx = SUBWINSIZEX;
	cy = SUBWINSIZEY;
	x = rcWin.right;
	y = rcWin.top;

	rc.left = 0;
	rc.top = 0;
	rc.right = cx;
	rc.bottom = cy;

	HWND		hParenthWnd = NULL;
	HINSTANCE	hInst = NULL;

	hParenthWnd = g_hWnd;
	hInst = GetModuleHandle(NULL);

	hWnd = CreateWindow(
		"sub",
		"sub",
		WS_POPUP | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, cx, cy, hParenthWnd, NULL,
		hInst, NULL);

	AdjustWindowRect(&rc, WINSTYLE, FALSE);

	SetWindowPos(hWnd, NULL, x, y,
		(rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER);

	ShowWindow(hWnd, SW_SHOW);
}