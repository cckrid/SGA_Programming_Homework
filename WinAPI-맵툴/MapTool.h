#pragma once

#include "GameNode.h"

// tile 21 x 16
#define MAXTILEX 21
#define MAXTILEY 16

#define MAXWINTILEX WINSIZEX / 25	// 40
#define MAXWINTILEY WINSIZEY / 25	// 30

struct tagTileInfo {
	RECT rc;
	int x, y;				// �����쿡�� Ÿ���� ��ġ x, y
	int tileX, tileY;		// �̹������� �о�� Ÿ�� x, y
	bool check;				// �׷������� üũ
};

class MapTool : public GameNode
{
public:
	MapTool();
	~MapTool();
};

