#pragma once

#include <Windows.h>

// getch() �� ���� �ƽ�Ű �ڵ尪 ����
#define ESC 27
#define SPACE 32
#define ENTER 13
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define UP_ARROW 72
#define DOWN_ARROW 80
#define SET_COLOR 2

class Color
{
public:
	Color();
	~Color();

	// ���� �־��ִ� �Լ�
	void SetColor(int color);
	// ���� ������ ��������ִ� �Լ�
	void SetDefaultColor();
};

