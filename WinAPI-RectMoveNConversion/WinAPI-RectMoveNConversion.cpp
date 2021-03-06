// WinAPI-RectMoveNConversion.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WinAPI-RectMoveNConversion.h"

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
    LoadStringW(hInstance, IDC_WINAPIRECTMOVENCONVERSION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIRECTMOVENCONVERSION));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIRECTMOVENCONVERSION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIRECTMOVENCONVERSION);
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

   HWND hWnd = CreateWindowW(szWindowClass, (LPCWSTR)"이미지 위치 및 크기 변환", 
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

RECT rect = { 150, 150, 250, 250 }; // 사각형

enum direction
{
	NONE, LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM
};

bool isRectMoved = false;			// 사각형이 움직이고 있는지
direction rectConvertedDir = NONE;		// 사각형의 크기가 바뀌고 있는지

POINT ptMouse = { 0,0 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[128];

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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			
			// 마우스 x,y 좌표 그리기
			sprintf_s(str, "X : %d, Y : %d", ptMouse.x, ptMouse.y);
			TextOut(hdc, 10, 10, str, strlen(str));

			sprintf_s(str, "사각형 : %d %d %d %d",
				rect.left, rect.top, rect.right, rect.bottom);
			TextOut(hdc, 10, 30, str, strlen(str));

			// 사각형이 움직이는 경우
			if (isRectMoved == true) {
				// 사각형의 위치를 마우스 포인터 기준으로 위치하도록 한다.
				// rect.right - rect.left = width
				// rect.bottom - rect.top = height
				// 마우스 포인터 중심이 사각형의 가운데 위치하도록
				LONG width = rect.right - rect.left; // 사각형의 너비
				LONG height = rect.bottom - rect.top; // 사각형의 높이

				rect.left = ptMouse.x - width / 2;
				rect.top = ptMouse.y - height / 2;
				rect.right = ptMouse.x + width / 2;
				rect.bottom = ptMouse.y + height / 2;
			}

			// 사각형이 변환되는 경우
			switch (rectConvertedDir)
			{
			case NONE:
				// test
				TextOut(hdc, 10, 50, "사각형 눌린 공간 : NONE",
					strlen("사각형 눌린 공간 : NONE"));
				break;
			case LEFTTOP:
				// test
				TextOut(hdc, 10, 50, "사각형 눌린 공간 : LEFTTOP",
					strlen("사각형 눌린 공간 : LEFTTOP"));
				rect.left = ptMouse.x;
				rect.top = ptMouse.y;
				break;
			case RIGHTTOP:
				// test
				TextOut(hdc, 10, 50, "사각형 눌린 공간 : RIGHTTOP",
					strlen("사각형 눌린 공간 : RIGHTTOP"));
				rect.right = ptMouse.x;
				rect.top = ptMouse.y;
				break;
			case LEFTBOTTOM:
				TextOut(hdc, 10, 50, "사각형 눌린 공간 : LEFTBOTTOM",
					strlen("사각형 눌린 공간 : LEFTBOTTOM"));
				rect.left = ptMouse.x;
				rect.bottom = ptMouse.y;
				break;
			case RIGHTBOTTOM:
				TextOut(hdc, 10, 50, "사각형 눌린 공간 : RIGHTBOTTOM",
					strlen("사각형 눌린 공간 : RIGHTBOTTOM"));
				rect.right = ptMouse.x;
				rect.bottom = ptMouse.y;
				break;
			}

			RectangleMake(hdc, rect);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_MOUSEMOVE:
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		if (Collision(ptMouse.x, ptMouse.y, rect)) {
			isRectMoved = true;
		}
		break;
	case WM_LBUTTONUP:
		isRectMoved = false;
		break;
	case WM_RBUTTONDOWN:
	{
		LONG width = rect.right - rect.left; // 사각형의 너비
		LONG height = rect.bottom - rect.top; // 사각형의 높이

		// 사각형 왼쪽 위 부분의 마우스가 위치해 있는지
		if (Collision(ptMouse.x, ptMouse.y,
			rect.left, rect.top, rect.left + width / 2,
			rect.top + height / 2)) {
			rectConvertedDir = LEFTTOP;
		}
		// 사각형 오른쪽 위 부분의 마우스가 위치해 있는지
		if (Collision(ptMouse.x, ptMouse.y,
			rect.left + width / 2, rect.top, rect.right,
			rect.top + height / 2)) {
			rectConvertedDir = RIGHTTOP;
		}
		// 사각형 왼쪽 아래 부분의 마우스가 위치해 있는지
		if (Collision(ptMouse.x, ptMouse.y,
			rect.left, rect.top + height / 2,
			rect.left + width / 2, rect.bottom)) {
			rectConvertedDir = LEFTBOTTOM;
		}
		// 사각형 오른쪽 아래 부분의 마우스가 위치해 있는지
		if (Collision(ptMouse.x, ptMouse.y,
			rect.left + width / 2, rect.top + height / 2,
			rect.right, rect.bottom)) {
			rectConvertedDir = RIGHTBOTTOM;
			
		}
	}
		break;
	case WM_RBUTTONUP:
		rectConvertedDir = NONE;
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
