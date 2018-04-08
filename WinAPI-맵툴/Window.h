#pragma once

#include "SIngletonBase.h"

// �̹��� ũ�� + ���� + (Y ���) SAVE LOAD
// 16, 40�� ���� �־���ϴ� ���� 40�� ���� �޴����̰� 16�� ���� �𸣰���
// Y�� �̴ϸ� ũ�� �߰� + 350
#define SUBWINSIZEX 21 * 20 + 16				// 436
#define SUBWINSIZEY 16 * 20 + 40 + 50 + 350		// 410 + 350 = 760

class Window : public SingletonBase<Window>
{
private:
	HWND hWnd;
	static POINT ptMouse;

	Image * m_backBuffer;
	GameNode * currentScene;

	bool isActive;
public:
	Window();
	~Window();

	void Init();
	void Release();
	void Update();
	void Render();
	void SetScene(GameNode* scene);
	static LRESULT CALLBACK WndLogProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	POINT GetPtMouse() { return ptMouse; }

	void SetIsActive(bool isActive) { this->isActive = isActive; }
	bool GetIsActive() { return isActive; }
};

#define SUBWIN Window::GetSingleton()
