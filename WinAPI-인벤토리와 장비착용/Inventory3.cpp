#include "stdafx.h"
#include "Inventory3.h"


Inventory3::Inventory3()
{
}


Inventory3::~Inventory3()
{
}

void Inventory3::ItemSetup()
{
	tagItemInfo item;
	item.itemKind = ITEM_MONSTERBALL;
	item.name = "���ͺ�";
	item.price = 200;
	item.attribute = 1;
	item.description = "�߻� ���ϸ󿡰� ������ ��� ���� ��. ĸ�������� �Ǿ��ִ�.";
	item.count = 5;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_PLUSPOWER;
	item.name = "�÷����Ŀ�";
	item.price = 500;
	item.attribute = 5;
	item.description = "���ϸ��� ���·��� 5 �ö󰣴�.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_DEFENDUP;
	item.name = "������";
	item.price = 550;
	item.attribute = 5;
	item.description = "���ϸ��� ������ 5 �ö󰣴�.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_SPECIALUP;
	item.name = "����Ⱦ�";
	item.price = 350;
	item.attribute = 5;
	item.description = "���ϸ��� Ư�� ���ݷ��� 5 �ö󰣴�.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_SPECIALGUARD;
	item.name = "����Ȱ���";
	item.price = 350;
	item.attribute = 5;
	item.description = "���ϸ��� Ư�� ������ 5 �ö󰣴�.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_SPEEDUP;
	item.name = "���ǵ��";
	item.price = 350;
	item.attribute = 5;
	item.description = "���ϸ��� ���ǵ尡 5 �ö󰣴�.";
	item.count = 1;

	m_vItem.push_back(item);

	// �̹��� ��� �Ⱦ����
	//item.itemKind = BOOSTER_ACCURACY;
	//item.name = "��-������";
	//item.price = 950;
	//item.attribute = 5;
	//item.description = "���ϸ��� ���߷��� 5 �ö󰣴�.";
	//item.count = 1;

	//m_vItem.push_back(item);

	item.itemKind = BOOSTER_CRITICALCUTTER;
	item.name = "ũ��Ƽ��Ŀ��";
	item.price = 350;
	item.attribute = 5;
	item.description = "���ϸ��� ũ����Ȯ���� 5 �ö󰣴�.";
	item.count = 1;

	m_vItem.push_back(item);

	item.itemKind = BOOSTER_EFFECTGUARD;
	item.name = "����Ʈ����";
	item.price = 700;
	item.attribute = 1;
	item.description = "���ϸ��� �ɷ�ġ�� �������� �ʴ´�.";
	item.count = 1;

	m_vItem.push_back(item);

	this->m_equipItem.itemKind = ITEM_EMPTY;
}

void Inventory3::AddItem(tagItemInfo item)
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

tagItemInfo Inventory3::SellItem(int num, int & gold)
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

void Inventory3::EquipItem(int num)
{
	if(num == -1){
		tagItemInfo beforeItem;
		beforeItem = GetEquipItem();

		tagItemInfo emptyItem;
		emptyItem.itemKind = ITEM_EMPTY;
		SetEquipItem(emptyItem);

		if (beforeItem.itemKind != ITEM_EMPTY)
			m_vItem.push_back(beforeItem);
	}
	else {
		tagItemInfo beforeItem;
		m_viItem = m_vItem.begin() + num;

		beforeItem = GetEquipItem();
		SetEquipItem(*m_viItem);
		m_vItem.erase(m_viItem);
		if (beforeItem.itemKind != ITEM_EMPTY)
			m_vItem.push_back(beforeItem);
	}
}
