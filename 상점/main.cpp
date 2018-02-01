#include <iostream>
#include <Windows.h>

#include "Shop.h"
#include "Customer.h"

#define SLEEP_TIME 1500

using namespace std;

bool buyItem(ITEM_TYPE category, Shop& shop, Customer& customer);
bool sellItem(Shop& shop, Customer& customer);

void main()
{
	Shop shop;
	Customer customer;
	int selectNumber;

	while (true) {
		system("cls");

		shop.ShowSelect();
		cin >> selectNumber;

		switch (selectNumber) {
		// ���� ����
		case 1:
			while (true) {
				system("cls");
				shop.ShowCategory();
				cin >> selectNumber;


				if (selectNumber == 0)
					break; 
				switch (selectNumber) {
				case 1:
					while (buyItem(ARMOUR, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				case 2:
					while (buyItem(WEAPON, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				case 3:
					while (buyItem(ACCESSORY, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				case 4:
					while (buyItem(POTION, shop, customer)) { Sleep(SLEEP_TIME); };
					break;
				}

			}
			break;
		// �κ��丮[�Ǹ�]
		case 2:
			while (sellItem(shop, customer)) { Sleep(SLEEP_TIME); };
			break;
		// �κ��丮[����]
		case 3:
			break;
		}
	}
}

bool buyItem(ITEM_TYPE category, Shop& shop, Customer& customer) {
	Item* items;
	Item* item;
	int gold;
	int selectNumber;
	int itemCount;

	system("cls");
	shop.ShowItem(category);
	items = shop.GetItems(category);

	itemCount = shop.GetItemCount(category);

	cout << "���� gold :" << customer.GetGold() << endl;
	cout << "������ �������� ��ȣ�� �Է��ϼ���." << endl;
	cout << "itmeCount : " << itemCount << endl;
	cout << "=== �����޴��� ������ ���ϸ� 0�� ===" << endl;
	cin >> selectNumber;

	if (selectNumber == 0)
		return false;
	else if (selectNumber > 0 && selectNumber <= itemCount) {
		item = &items[selectNumber - 1];
		// ��� �Ǵ� ��� �ݾ� ����, ������ ����
		if (shop.Buy(category, selectNumber - 1, customer.GetGold())) {
			gold = customer.GetGold();
			customer.SetGold(gold - item->GetPrice());
			customer.InsertItem(item);
		}
	}
	return true;
}

bool sellItem(Shop& shop, Customer& customer) {
	int selectNumber;
	int gold;

	system("cls");
	customer.ShowInventory();

	cin >> selectNumber;

	if (selectNumber == 0)
		return false;
	else if(selectNumber > 0 && selectNumber <= customer.GetItemCount()) {
		Item* item = customer.GetItem(selectNumber - 1);
		customer.DeleteItem(selectNumber - 1);
		gold = customer.GetGold();
		customer.SetGold(gold + item->GetPrice() / 2);	// ���ݰ� ��ȯ
		int id = shop.GetItemIndex(item->GetItemType(), item->GetName());
		shop.Sell(item->GetItemType(), id);
	}
}