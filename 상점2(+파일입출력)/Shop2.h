#pragma once

#include "Item.h"
#include "Color.h"

#include <stdio.h>
#include <string.h>

class Shop2 : public Color
{
	// ����ü�� ���� ����
	vector<tagItemInfo> m_vItem;
	vector<tagItemInfo>::iterator m_viItem;

public:
	Shop2();
	~Shop2();

	// �ʱ� ������ ����
	bool ItemSetup();
	// ������ ���
	int ItemOutput(int num, int selectNum);
	// ���� ������ ����
	tagItemInfo BuyItem(int itemKind, int num, int& gold);
	// ������ ������ �߰�
	void AddItem(tagItemInfo item);
};

