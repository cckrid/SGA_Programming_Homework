// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 이렇게하면 PI 쓸 수 있음
#define _USE_MATH_DEFINES
#include <math.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "CommonMacroFunction.h"
#include "KeyManager.h"
#include "RandomFunction.h"

// define 
#define WINNAME (LPTSTR)(TEXT("WinAPI - 에어 하키"))
#define WINSTARTX 0
#define WINSTARTY 0
#define WINSIZEX 1024
#define WINSIZEY 768
#define WINSTYLE WS_CAPTION | WS_SYSMENU

// macro
#define SAFE_DELETE(p) {if(p) {delete p; (p)=NULL;}}
#define SAFE_RELEASE(p) {if(p) {(p)->Release(); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[] (p); (p)=NULL;}}

#define PI 3.141592

// extern
extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern POINT g_ptMouse;