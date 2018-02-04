#pragma once

#include <Windows.h>
#include <conio.h>
#include "Shop2.h"
#include "Player2.h"
#include "Color.h"

// ��ġ
enum LOCATION {
	LOCATION_SHOP = 1,
	LOCATION_SELL_INVENTORY,
	LOCATION_INVENTORY
};

class MainGame : public Color
{
private:
	Shop2* m_shop;
	Player2* m_player;
	LOCATION m_location;

	int m_gold;
	int m_selectNum;
	int m_selectPtr;
public:
	MainGame();
	~MainGame();
	
	// ����, �κ��丮 �̵�
	void SetLocation(int number);
	// ���� ȭ��
	void SetMainPage();
	// ���� ȭ��
	void SetShopPage();
	// ������ �������� �����ִ� �Լ�
	void SetShopItemPage(int itemKind);
	// �κ��丮 ���� �ǸŸ� �����ִ� �Լ�
	void SetSellInventoryPage();
	// �κ��丮 �����ִ� �Լ� (������)
	void SetInventoryPage();
};

