#pragma once

#include "Item.h"
#include <stdio.h>
#include <string.h>

class Shop2
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
	int ItemOutput(int num);
	// ���� ������ ����
	tagItemInfo BuyItem(int itemKind, int num, int& gold);
	// ������ ������ �߰�
	void AddItem(tagItemInfo item);
};

