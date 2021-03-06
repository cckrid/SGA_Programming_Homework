// WinApi-GuGuDan.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WinApi-GuGuDan.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIGUGUDAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIGUGUDAN));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	// 여기다 CS_DBLCLKS 를 넣어줘야 더블클릭 메시지 지원한다
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIGUGUDAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIGUGUDAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, (LPCWSTR)"구구단 게임", 
	   WS_OVERLAPPEDWINDOW,
      0, 0, 1024, 768, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// 전역 변수
int number;
RECT rc;
char str[256];
int nameX = 300;
int nameY = 300;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
	{
		number = 0;
		rc.left = 150;
		rc.top = 150;
		rc.bottom = 500;
		rc.right = 300;
	}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.

			TextOut(hdc, 500, 500, str, strlen(str));

			char temp[128];

			if (number < 1)
				number = 1;

			for (int i = 0; i < 9; i++) {
				sprintf_s(temp, "%d X %d = %d", number, i + 1, number*(i + 1));
				TextOut(hdc, 150, 150 + i * 20, temp, strlen(temp));
			}

			// 이름 쓰기 김태훈
			int tempNameX;
			int tempNameY;
			// ㄱ
			MoveToEx(hdc, nameX, nameY, NULL);
			LineTo(hdc, nameX + 10, nameY + 10);
			LineTo(hdc, nameX, nameY + 30);
			tempNameX = nameX + 25;
			tempNameY = nameY;
			// l
			MoveToEx(hdc, tempNameX, nameY, NULL);
			LineTo(hdc, tempNameX, nameY + 30);
			// ㅁ
			tempNameX = nameX + 5;
			tempNameY = nameY + 35;
			int t = 20;
			MoveToEx(hdc, tempNameX, tempNameY, NULL);
			LineTo(hdc, tempNameX + t, tempNameY);
			LineTo(hdc, tempNameX + t, tempNameY + t);
			LineTo(hdc, tempNameX, tempNameY + t);
			LineTo(hdc, tempNameX, tempNameY);
			// ㅌ
			nameX += 40;
			nameY += 10;
			MoveToEx(hdc, nameX, nameY, NULL);
			LineTo(hdc, nameX, nameY + 30);
			MoveToEx(hdc, nameX, nameY, NULL);
			LineTo(hdc, nameX + t, nameY);
			MoveToEx(hdc, nameX, nameY + 15, NULL);
			LineTo(hdc, nameX + t, nameY + 15);
			MoveToEx(hdc, nameX, nameY + 30, NULL);
			LineTo(hdc, nameX + t, nameY + 30);
			// ㅐ
			nameX += 25;
			nameY -= 10;
			MoveToEx(hdc, nameX, nameY + 10, NULL);
			LineTo(hdc, nameX, nameY + 45);
			MoveToEx(hdc, nameX, nameY + 25, NULL);
			LineTo(hdc, nameX + t / 2, nameY + 25);
			nameX += t / 2;
			MoveToEx(hdc, nameX, nameY, NULL);
			LineTo(hdc, nameX, nameY + 50);
			// ㅎ
			nameX += 20;
			MoveToEx(hdc, nameX, nameY, NULL);
			LineTo(hdc, nameX + t/2, nameY);
			nameY += 10;
			MoveToEx(hdc, nameX - 10, nameY, NULL);
			LineTo(hdc, nameX + t, nameY);
			Ellipse(hdc, nameX - 3, nameY + 5, nameX - 3 + 15, nameY + 5 + 15);
			// ㅜ
			nameX -= 10;
			nameY += 25;
			MoveToEx(hdc, nameX, nameY, NULL);
			LineTo(hdc, nameX + 30, nameY);
			MoveToEx(hdc, nameX + 15, nameY, NULL);
			LineTo(hdc, nameX + 15, nameY + t/2);
			// ㄴ
			nameX += 5;
			nameY += 8;
			MoveToEx(hdc, nameX, nameY, NULL);
			LineTo(hdc, nameX, nameY + t/2);
			LineTo(hdc, nameX + t, nameY + t / 2);

			nameX = 300;
			nameY = 300;

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
	{
		HDC hdc = GetDC(hWnd);
		char temp[128];

		number++;
		InvalidateRect(hWnd, &rc, TRUE);
		TextOut(hdc, 600, 600, "테스트", strlen("테스트"));
		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_RBUTTONDOWN:
	{
		HDC hdc = GetDC(hWnd);

		number--;
		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hdc);
	}
	break;
	// 더블 클릭 할 수 있게 해주는거 이거 하려면 wcex.style에 넣어줘야하는거 있음
	//case WM_LBUTTONDBLCLK:
	//	InvalidateRect(hWnd, NULL, TRUE);
	//	break;
	// 문자열 입력받는 방법
	case WM_CHAR:
	{
		//int len;
		//len = strlen(str);
		//str[len] = (TCHAR)wParam;
		//str[len + 1] = 0;

		str[0] = (TCHAR)wParam;
		str[1] = 0;

		HDC hdc = GetDC(hWnd);

		InvalidateRect(hWnd, NULL, TRUE);

		ReleaseDC(hWnd, hdc);
	}
	break;
	case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
