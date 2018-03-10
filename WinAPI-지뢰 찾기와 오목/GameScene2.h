#pragma once
#include "GameNode.h"

enum GomokuSTATUS {
	GOMOKU_EMPTY,
	GOMOKU_BLACK,
	GOMOKU_WHITE
};

struct tagGomokuInfo {
	int x, y;
	RECT rc;
	GomokuSTATUS status;
};

class GameScene2 : public GameNode
{
private:
	bool isDebug;
	char str[128];

	Image * map;
	Image * menu;
	Image* black;
	Image* white;

	// ���� ������ �� �÷��̾� ������ ���� ����
	GomokuSTATUS currentGomoku;
	// ���� ���� ������ ���� ����
	GomokuSTATUS winner;

	tagGomokuInfo gomoku[19][19];
	bool isOver;

	RECT rcGomokuExit;
public:
	GameScene2();
	~GameScene2();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void CheckGomoku(int row, int col, GomokuSTATUS current);
	void GameOver(GomokuSTATUS winner);
};

