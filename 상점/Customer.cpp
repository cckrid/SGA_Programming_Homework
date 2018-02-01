#include "Customer.h"



Customer::Customer()
{
	gold = 10000;
}


Customer::~Customer()
{
}

void Customer::ShowInventory()
{
	cout << "================ �κ��丮 ================" << endl;
	cout << "���� gold : " << gold << endl;
	cout << "�Ǹ��� �������� ��ȣ�� �Է��ϼ���." << endl;
	cout << "=== ����ȭ������ ������ ���ϸ� 0�� ===" << endl;
	for (int i = 0; i<inventory.size(); i++) {
		inventory[i]->MakeLine();
		cout << "������ ��ȣ : " << i + 1 << endl;
		inventory[i]->Show();
		inventory[i]->MakeLine();
	}
}

int Customer::GetGold()
{
	return gold;
}

void Customer::SetGold(int gold)
{
	this->gold = gold;
}

void Customer::InsertItem(Item* item)
{
	inventory.push_back(item);
}

void Customer::DeleteItem(int id)
{
	inventory.erase(inventory.begin() + id);
}

Item* Customer::GetItem(int index)
{
	return inventory[index];
}

int Customer::GetItemCount()
{
	return inventory.size();
}
