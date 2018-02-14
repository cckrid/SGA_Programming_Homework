#pragma once

#include "GameNode.h"

struct RectInfo {
	RECT rect;
	float speed;	// rect �ӵ�
};

struct LevelInfo{
	int	count;		// rect ���� level �������� ������ ������
	int size;		// rect ũ�� level �������� �۾���
	float speed;	// rect �ӵ� level �������� �ӵ��� ������
};

enum LEVEL {
	LEVEL_ZERO,		// �⺻���� ���� rect�� ������
	LEVEL_ONE,		// rect �� ����
	LEVEL_TWO,		// rect �ӵ� ����
	LEVEL_END		// �ְ� ����, ũ��� �ӵ� ����
};

class MainGame2 : public GameNode
{
private:
	RECT player;
	
	RectInfo rcInfo[100];	// Level�� ���� rect ���� �����ϴ� ����ü
	LevelInfo lvInfo[4];	// Level ���� �����ϴ� ����ü

	LEVEL currentLevel;

	bool isLevelUp;
	bool isGameOver;

	int score;

	int timeCount;
	float prevTime;
	float currentTime;
public:
	MainGame2();
	~MainGame2();

	void SetupLevelInfo();			// levelInfo setup
	void SetupRectInfo();			// rectInfo setup level up�õ� ����

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;
};

