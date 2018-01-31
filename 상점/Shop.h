#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "Weapon.h"
#include "Armour.h"
#include "Potion.h"
#include "Accessory.h"

using namespace std;

class Shop
{
private:
	vector<Item> weapon;
	vector<Item> armour;
	vector<Item> potion;
	vector<Item> acce;
public:
	Shop();
	~Shop();

	// ����, �Ǹ� ������ �ֿܼ� �����ִ� �Լ�
	void ShowSelect();
	// ��, ����, �Ǽ�����, ���� ��� ������ �ֿܼ� �����ִ� �Լ�
	void ShowCategory();

	// Ư�� ��Ͽ� �������� �����ִ� �Լ�
	void ShowItem(ITEM_TYPE category);
	// Ư�� ��Ͽ� �������� ��� �Լ� ��� �Ǵ��� �ƴ��� true/false ��ȯ
	bool Buy(ITEM_TYPE category, int id, int gold);
	// Ư�� ��Ͽ� �������� �Ĵ� �Լ�
	void Sell(ITEM_TYPE category, int id);

	// ������ ����� �Լ� (�ϵ��ڵ� == �밡��)
	void MakeWeapon();
	void MakeArmour();
	void MakePotion();
	void MakeAcce();
	// �������� �о���� �Լ� ����� �ϵ��ڵ��� ������ �о��
	void ReadItem(ITEM_TYPE category);

	// Ư�� ��Ͽ� �������� ��� vector�� ��ȯ�ϴ� �Լ�
	vector<Item>& GetItems(ITEM_TYPE category);
	// Ư�� ��Ͽ� ��� �������� Index�� ��ȯ�ϴ� �Լ�
	int GetItemIndex(ITEM_TYPE category, string name);

	void Init();	// Shop Ŭ���� �ʱ�ȭ (������ �о��)
	//void Release();
	//void Update();
	//void Render();
};

