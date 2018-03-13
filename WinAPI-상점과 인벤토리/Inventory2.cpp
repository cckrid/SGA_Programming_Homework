#include "stdafx.h"
#include "Inventory2.h"


Inventory2::Inventory2()
{
}


Inventory2::~Inventory2()
{
}

void Inventory2::ItemSetup()
{
	tagItemInfo item;
	item.itemKind = ITEM_MONSTERBALL;
	item.name = "���ͺ�";
	item.price = 200;
	item.attribute = 1;
	item.description = "�߻� ���ϸ󿡰� ������ ��� ���� ��. ĸ�������� �Ǿ��ִ�.";
	item.count = 5;

	m_vItem.push_back(item);
}

void Inventory2::AddItem(tagItemInfo item)
{
	bool isNewItem = true;

	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// ������ ���� �������� Ȯ��
		if (m_viItem->itemKind != item.itemKind) continue;

		// strcmp ���� string �ȿ� compare �Լ� ����
		// ������ �̸� ������ ������ ����
		if (m_viItem->name.compare(item.name) == 0) {
			m_viItem->count++;
			isNewItem = false;
			break;
		}
	}

	if (isNewItem == true) {
		m_vItem.push_back(item);
	}
}

tagItemInfo Inventory2::SellItem(int num, int & gold)
{
	tagItemInfo sellItem;
	m_viItem = m_vItem.begin() + num;
	sellItem = *m_viItem;

	if (m_viItem->count > 1)
		m_viItem->count--;
	else
		m_vItem.erase(m_viItem);

	int sellPrice = sellItem.price / 2;
	gold += sellPrice;

	return sellItem;
}
