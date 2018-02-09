#pragma once

//////////////////////////////////////////////////////////////////
// CommonMacroFunction (�ʿ��� �κ��� ���� ���� �߰� �Ұ�)
//////////////////////////////////////////////////////////////////

// POINT -> (LONG x, LONG y) �Ǿ� �ִ� ����ü

// inline �ߺ����� ���ϴ°�

inline POINT PointMake(int x, int y) {
	POINT pt = { x, y };
	return pt;
}

inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

inline RECT RectMake(int x, int y, int width, int height) {
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

inline void RectangleMake(HDC hdc, int x, int y, int width, int height) {
	Rectangle(hdc, x, y, x + width, y + height);
}

inline void RectangleMake(HDC hdc, RECT rect) {
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

inline bool Collision(RECT rect1, RECT rect2) {

	if ((rect1.left <= rect2.left && rect2.left <= rect1.right)
		|| (rect1.left <= rect2.right && rect2.right <= rect1.right)) {
		if ((rect1.top <= rect2.top && rect2.top <= rect1.bottom)
			|| (rect1.top <= rect2.bottom && rect2.bottom <= rect1.bottom)) {
			return true;
		}
	}
	return false;
}

inline bool InnerCollision(RECT rect1, RECT rect2, int dir) {
	switch (dir) {
	case VK_LEFT:
		if (rect1.right <= rect2.right)
			return true;
		else
			return false;
		break;
	case VK_RIGHT:
		if (rect1.left >= rect2.left)
			return true;
		else
			return false;
		break;
	case VK_UP:
		if (rect1.bottom <= rect2.bottom)
			return true;
		else
			return false;
		break;
	case VK_DOWN:
		if (rect1.top >= rect2.top)
			return true;
		else
			return false;
		break;
	default:
		return false;
		break;
	}
}