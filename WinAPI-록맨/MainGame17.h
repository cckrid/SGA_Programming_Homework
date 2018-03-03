#pragma once

#include "GameNode.h"

/*
	�ϸ� ��������Ʈ ����
	12 x 6
	���� �ȱ� 11��
	������ �ȱ� 11��
	���� ��� 6�� ���ʺ���� 3��
	������ ��� 6�� �����ʺ���� 3��
	�������� 12��
	������ ���� 12��
*/

enum RockmanState {
	STATE_LEFTIDLE,
	STATE_RIGHTIDLE,
	STATE_LEFTJUMP,
	STATE_RIGHTJUMP,
	STATE_LEFTWALLCATCH,
	STATE_LEFTWALK,
	STATE_RIGHTWALLCATCH,
	STATE_RIGHTWALK
};

struct tagRockmanInfo {
	RockmanState state;
	bool isGround;
};

class MainGame17 : public GameNode
{
private:
	Image * rockman;
	Image * bg;

	tagRockmanInfo rockmanInfo;

	RECT ground[3];

	float gravity;

	bool checkRect;
	bool checkStatus;

	DWORD prevTime;
	DWORD curTime;
public:
	MainGame17();
	~MainGame17();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

