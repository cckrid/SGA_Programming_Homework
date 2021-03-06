// WinAPI-Collision.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WinAPI-Collision.h"

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
    LoadStringW(hInstance, IDC_WINAPICOLLISION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPICOLLISION));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPICOLLISION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPICOLLISION);
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

   HWND hWnd = CreateWindowW(szWindowClass, (LPCWSTR)"이미지 충돌 처리", 
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

RECT rect1 = { 150, 150, 250, 250 }; // 사각형 1
RECT rect2 = { 300, 300, 400, 400 }; // 사각형 2
RECT rect3 = { 180, 180, 220, 220 }; // 사각형 3

bool isControlRect1 = true;
bool isControlRect3 = false;

int direction; // 방향키의 방향을 저장

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

			// x, y 좌표 그리기
			sprintf_s(str, "X : %d, Y : %d", ptMouse.x, ptMouse.y);
			TextOut(hdc, 10, 10, str, strlen(str));

			// 각 사각형들의 좌표 그리기
			for (int i = 0; i < 3; i++) {
				switch (i) {
				case 0:
					sprintf_s(str, "사각형 %d : (%d, %d, %d, %d)",
						i + 1, rect1.left, rect1.top, rect1.right, rect1.bottom);
					TextOut(hdc, 10, 50 + i * 20, str, strlen(str));
					break;
				case 1:
					sprintf_s(str, "사각형 %d : (%d, %d, %d, %d)",
						i + 1, rect2.left, rect2.top, rect2.right, rect2.bottom);
					TextOut(hdc, 10, 50 + i * 20, str, strlen(str));
					break;
				case 2:
					sprintf_s(str, "사각형 %d : (%d, %d, %d, %d)",
						i + 1, rect3.left, rect3.top, rect3.right, rect3.bottom);
					TextOut(hdc, 10, 50 + i * 20, str, strlen(str));
					break;
				}

			}

			if (isControlRect1) {
				TextOut(hdc, 10, 30, "= 사각형 1 움직이는 중 =", 
					strlen("= 사각형 1 움직이는 중 ="));
			}
			else
				TextOut(hdc, 10, 30, "= 사각형 2 움직이는 중 =",
					strlen("= 사각형 2 움직이는 중 ="));

			// 두 사각형 그리기
			// CommonMacroFunction에 만들어 둠 rect만 넣음 그려지게
			RectangleMake(hdc, rect1);
			RectangleMake(hdc, rect2);
			RectangleMake(hdc, rect3);

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_MOUSEMOVE:
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			direction = VK_LEFT;
			if (isControlRect1) {
				rect1.left -= 5.0f;
				rect1.right -= 5.0f;
			}
			else {
				rect2.left -= 5.0f;
				rect2.right -= 5.0f;
			}

			//if (isControlRect3) {
			if(InnerCollision(rect1, rect3)) {
				rect3.left -= 5.0f;
				rect3.right -= 5.0f;
			}
			break;
		case VK_RIGHT:
			direction = VK_RIGHT;
			if (isControlRect1) {
				rect1.left += 5.0f;
				rect1.right += 5.0f;
			}
			else {
				rect2.left += 5.0f;
				rect2.right += 5.0f;
			}
			
			//if (isControlRect3) {
			if(InnerCollision(rect1, rect3)) {
				rect3.left += 5.0f;
				rect3.right += 5.0f;
			}
			break;
		case VK_UP:
			direction = VK_UP;
			if (isControlRect1) {
				rect1.top -= 5.0f;
				rect1.bottom -= 5.0f;
			}
			else {
				rect2.top -= 5.0f;
				rect2.bottom -= 5.0f;
			}

			//if (isControlRect3) {
			if(InnerCollision(rect1, rect3)) {
				rect3.top -= 5.0f;
				rect3.bottom -= 5.0f;
			}
			break;
		case VK_DOWN:
			direction = VK_DOWN;
			if (isControlRect1) {
				rect1.top += 5.0f;
				rect1.bottom += 5.0f;
			}
			else {
				rect2.top += 5.0f;
				rect2.bottom += 5.0f;
			}

			//if (isControlRect3) {
			if(InnerCollision(rect1, rect3)) {
				rect3.top += 5.0f;
				rect3.bottom += 5.0f;
			}
			break;
		default:
			break;
		}

		// 사각형 1과 2가 충돌했을 때 컨트롤 체인지
		if (Collision(rect1, rect2)) {
			// 사각형 1을 움직일 때
			if (isControlRect1) {
				isControlRect1 = false;
				// 사각형 3의 위치를 사각형 2의 가운데 위치로 옮김
				rect3.left = rect2.left + 30;
				rect3.top = rect2.top + 30;
				rect3.right = rect2.right - 30;
				rect3.bottom = rect2.bottom - 30;
			}
			// 사각형 2를 움직일 때
			else {
				isControlRect1 = true;
				// 사각형 3의 위치를 사각형 1의 가운데 위치로 옮김
				rect3.left = rect1.left + 30;
				rect3.top = rect1.top + 30;
				rect3.right = rect1.right - 30;
				rect3.bottom = rect1.bottom - 30;
			}
		}

		// 방향까지 확인하여 내부 충돌 확인
		//// 사각형 1 움직이고 있을 때
		//if (isControlRect1) {
		//	
		//	if (InnerCollision(rect1, rect3, direction)) {
		//		isControlRect3 = true;
		//	}
		//	else
		//		isControlRect3 = false;
		//}
		//// 사각형 2 움직이고 있을 때
		//else {
		//	
		//	if (InnerCollision(rect2, rect3, direction)) {
		//		isControlRect3 = true;
		//	}
		//	else
		//		isControlRect3 = false;
		//}

		// 스위치문 밖에서 이런 식으로 관리하면 안됨
		// 스위치문 안에서 해야함
		//if (isControlRect1) {
		//	isControlRect3 = InnerCollision(rect1, rect3);
		//}
		//else
		//	isControlRect3 = InnerCollision(rect2, rect3);

		InvalidateRect(hWnd, NULL, TRUE);
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
