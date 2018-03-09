#include "stdafx.h"
#include "MainGame21.h"


MainGame21::MainGame21()
{
}


MainGame21::~MainGame21()
{
}

HRESULT MainGame21::Init()
{
	GameNode::Init();

	_bg = new Image;
	_bg->Init("images/stair.bmp", WINSIZEX, WINSIZEY);

	_ball = new Image;
	// ũ�Ⱑ ���� ��
	_ball->Init("images/ball.bmp", 60, 60, true, RGB(255, 0, 255));
	// ũ�Ⱑ ū ��
	//_ball->Init("images/ball.bmp", 100, 100, true, RGB(255, 0, 255));

	// ���� ���� ��ġ
	_x = 30;
	_y = WINSIZEY - 175;

	// �׸��� ���ϰ� �ϱ� ���� ���� ���� Rect�� ���ϱ�
	_rc = RectMakeCenter(_x, _y, _ball->GetWidth(), _ball->GetHeight());

	// ���� �عٴڿ��� Ž��
	_probeY = _y + _ball->GetHeight() / 2;
	// ���� ���� ������ Ž��
	_probeX = _x + _ball->GetWidth() / 2;

	isDebug = false;
	// �����̴��� Ȯ�� �ϱ� ���� ����
	isMove = false;

	return S_OK;
}

void MainGame21::Release()
{
	GameNode::Release();

	SAFE_DELETE(_bg);
	SAFE_DELETE(_ball);
}

void MainGame21::Update()
{
	GameNode::Update();

	// ����Ű�� ���� �����̰� �����϶� bool �� ���� true 
	if (INPUT->GetKey(VK_LEFT)) { _x -= 2.0f; isMove = true; }
	// �������� ������ false
	if (INPUT->GetKeyUp(VK_LEFT)) { isMove = false; }
	if (INPUT->GetKey(VK_RIGHT)) { _x += 2.0f; isMove = true; }
	if (INPUT->GetKeyUp(VK_RIGHT)) { isMove = false; }

	// Y�� Ž�� ���� (���� �ٴ�) �ֽ�ȭ
	_probeY = _y + _ball->GetHeight() / 2;

	// �������� �ϴ��� Ȯ���ϴ� ����
	bool isFall = true;

	// �عٴ� - 10 (��) ���� �عٴ� + 10���� Ž��
	for (int i = _probeY - 10; i < _probeY + 10; i++) {
		// �ش� ��ġ�� �ȼ� �� �о��
		COLORREF color = GetPixel(_bg->GetMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		
		// �ش� �ȼ��̸� (������ �ִ� �κ��̸�)
		if (r == 185 && g == 122 && b == 86) {
			// ���� y���� Ž���� ��ġ���� ���� ������ �ݸ�ŭ �ø�
			// ���� ������ �ݸ�ŭ �ø��� ���� y ����
			_y = i - _ball->GetHeight() / 2;
			// �������� �ʾƾ� �ϹǷ� false
			isFall = false;
			break;
		}
	}

	// y���� ���� Ž���� �ߴµ� Ž�� ���� �ʾ����� isFall true �̹Ƿ� 10��ŭ ������
	if (isFall) {
		_y += 10;
	}

	// ���� X�� Ž�� ���� �ֽ�ȭ (���� ������ ������ Ž��)
	_probeX = _x + _ball->GetWidth() / 2;

	// Ž������ - 10 ���� X �� Ž�� ������
	for (int i = _probeX - 10; i < _probeX; i++) {
		COLORREF color = GetPixel(_bg->GetMemDC(), i, _y - _ball->GetWidth() / 2);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		// �ش� �ȼ��̸�
		if (r == 185 && g == 122 && b == 86) {
			// ���� ���� x�� Ž���� ��ġ���� �ʺ�/2�� �� (x �� ����)
			_x = i - _ball->GetWidth() / 2;
			break;
		}
	}

	// ����� x, y �� ���� rect �ٽ� ����
	_rc = RectMakeCenter(_x, _y, _ball->GetWidth(), _ball->GetHeight());

	//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_F11)) {
		isDebug = !isDebug;
	}
	//==================================================//
}

void MainGame21::Render(HDC hdc)
{
	HDC memDC = GetBackBuffer()->GetMemDC();
	PatBlt(memDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//=================================================
	{
		_bg->Render(memDC);
		_ball->Render(memDC, _rc.left, _rc.top);

		char str[128];
		if (isMove) {
			sprintf_s(str, "�����̴� ��");
		}
		else {
			sprintf_s(str, "���� ��");
		}
		TextOut(memDC, 10, 10, str, strlen(str));
	}
	//==================   Debug   ====================
	if (isDebug)
	{

	}
	//=================================================
	this->GetBackBuffer()->Render(hdc);
}
