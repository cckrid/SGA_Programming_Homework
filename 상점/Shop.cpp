#include "Shop.h"



Shop::Shop()
{
	Init();
}


Shop::~Shop()
{
	delete[] weapon;
	delete[] armour;
	delete[] potion;
	delete[] acce;
}

void Shop::ShowSelect()
{
	cout << "================ ��ġ ================" << endl;
	cout << "1. ���� 2. �κ��丮[�Ǹ�] 3. �κ��丮[����]" << endl;
}

void Shop::ShowCategory()
{
	cout << "================ ���� ================" << endl;
	cout << "1. �� 2. ���� 3. �Ǽ����� 4. ����" << endl;
	cout << "=== ����ȭ������ ������ ���ϸ� 0�� ===" << endl;
}

void Shop::ShowItem(ITEM_TYPE category)
{
	switch (category)
	{
	case WEAPON:
		for (int i=0; i<weaponSize; i++){
			weapon[i].MakeLine();
			cout << "������ ��ȣ : " << i + 1 << endl;
			weapon[i].Show();
			weapon[i].MakeLine();
		}
		break;
	case ARMOUR:
		for (int i = 0; i<armourSize; i++) {
			armour[i].MakeLine();
			cout << "������ ��ȣ : " << i + 1 << endl;
			armour[i].Show();
			armour[i].MakeLine();
		}
		break;
	case POTION:
		for (int i = 0; i<potionSize; i++) {
			potion[i].MakeLine();
			cout << "������ ��ȣ : " << i + 1 << endl;
			potion[i].Show();
			weapon[i].MakeLine();
		}
		break;
	case ACCESSORY:
		for (int i = 0; i<acceSize; i++) {
			acce[i].MakeLine();
			cout << "������ ��ȣ : " << i + 1 << endl;
			acce[i].Show();
			acce[i].MakeLine();
		}
		break;
	}
}

bool Shop::Buy(ITEM_TYPE category, int id, int gold)
{
	int count;
	string name;
	int price;
	// ������ ������ �־ �� �� �ִ��� Ȯ���ϴ� ����
	bool isCount = true;
	bool isMoney = true;
	switch (category)
	{
	case WEAPON:
		count = weapon[id].GetCount();
		price = weapon[id].GetPrice();
		name = weapon[id].GetName();
		if (count > 0 && gold >= price) {
			weapon[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	case ARMOUR:
		count = armour[id].GetCount();
		name = armour[id].GetName();
		price = armour[id].GetPrice();
		if (count > 0 && gold >= price) {
			armour[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	case POTION:
		count = potion[id].GetCount();
		name = potion[id].GetName();
		price = potion[id].GetPrice();
		if (count > 0 && gold >= price) {
			potion[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	case ACCESSORY:
		count = acce[id].GetCount();
		name = acce[id].GetName();
		price = acce[id].GetPrice();
		if (count > 0 && gold >= price) {
			acce[id].SetCount(count - 1);
		}
		else if (count <= 0)
			isCount = false;
		else if (gold <= price)
			isMoney = false;
		break;
	}

	if (isCount == true && isMoney) {
		cout << name << "�� " << price << "gold" << "�� �����Ͽ����ϴ�" << endl;
		return true;
	}
	else if (isCount == false) {
		cout << "��� ��� ���� �� �� �����ϴ�." << endl;
		return false;
	}
	else if (isMoney == false) {
		cout << "���� �����մϴ�." << endl;
		return false;
	}
}

void Shop::Sell(ITEM_TYPE category, int id)
{
	int count;
	string name;
	int price;

	switch (category)
	{
	case WEAPON:
		count = weapon[id].GetCount();
		name = weapon[id].GetName();
		price = weapon[id].GetPrice();
		weapon[id].SetCount(count + 1);
		break;
	case ARMOUR:
		count = armour[id].GetCount();
		name = armour[id].GetName();
		price = armour[id].GetPrice();
		armour[id].SetCount(count + 1);
		break;
	case POTION:
		count = potion[id].GetCount();
		name = potion[id].GetName();
		price = potion[id].GetPrice();
		potion[id].SetCount(count + 1);
		break;
	case ACCESSORY:
		count = acce[id].GetCount();
		name = acce[id].GetName();
		price = acce[id].GetPrice();
		acce[id].SetCount(count + 1);
		break;
	}

	cout << name << "�� " << price / 2 << "gold�� �Ǹ��Ͽ����ϴ�." << endl;
}

void Shop::MakeWeapon()
{
	weaponSize = 5;
	weapon = new Weapon[weaponSize];
	weapon[0].Init(
		WEAPON, "������ ��", 500, 4,
		"�� �ٵ���� ����Į. ���԰� �� �������־� �ʺ��ڰ� �ٷ�� ����.", 1
	);
	weapon[1].Init(
		WEAPON, "�� �ҵ�", 2000, 7,
		"���� �ٷ�� ���� �������� ������ �� �� �ִ� ����", 1
	);
	weapon[2].Init(
		WEAPON, "�� �ҵ�", 3000, 9,
		"���̰� ��鼭�� �������� ���ݷ��� �ڶ��ϴ� ����.", 1
	);
	weapon[3].Init(
		WEAPON, "�� �ڵ�� �ҵ�", 7000, 25,
		"���� ����� Ű���� Ŭ ������ ������� �Ѽ����δ� ��� �� �� ����.", 1
	);
	weapon[4].Init(
		WEAPON, "Ŭ���̸��", 8000, 28,
		"�β��� ��� ź�¼��� �ִ� ������ ���� ���� ���� ���� ������� ��հ�.", 1
	);
}

void Shop::MakeArmour()
{
	armourSize = 5;
	armour = new Armour[armourSize];
	armour[0].Init(
		ARMOUR, "�ʺ��ڿ� ����", 50, 1,
		"�ʺ��ڰ� ���� �Դ� ������ ���̴�.", 1
	);
	armour[1].Init(
		ARMOUR, "��ɲۿ�", 200, 2,
		"���׼����� ���ǿ� ưư�� õ���� �� ������ �̷�����ִ�.", 1
	);
	armour[2].Init(
		ARMOUR, "����Ʈ ��������", 600, 4,
		"��¥ ������ ��� ���� ���װ���", 1
	);
	armour[3].Init(
		ARMOUR, "ü�θ���", 2000, 8,
		"����� ���� �罽�� ���� �����̴�.", 1
	);
	armour[4].Init(
		ARMOUR, "�÷���Ʈ����", 4000, 14,
		"ö���� ����� ���� ưư�� �����̴�.", 1
	);
}

void Shop::MakePotion()
{
	potionSize = 5;
	string hp[5] = { "10", "30", "50", "100", "300" };
	int price[5] = { 50,200,400,1000,4000 };
	potion = new Potion[potionSize];
	for (int i = 0; i < potionSize; i++) {
		potion[i].Init(
			POTION, "����� " + hp[i] + " ����", price[i], atoi(hp[i].c_str()),
			"������� " + hp[i] + " ȸ���� �� �ִ� ����", 1
		);
	}
}

void Shop::MakeAcce()
{
	acceSize = 5;
	acce = new Accessory[acceSize];
	acce[0].Init(
		ACCESSORY, "�콼 ����", 50, 1,
		"���� ���� ���� �Ҿ���� ����", 1
	);
	acce[1].Init(
		ACCESSORY, "����", 100, 2,
		"�����ϰ� ���� ������ ����", 1
	);
	acce[2].Init(
		ACCESSORY, "������", 500, 3,
		"������ ������� ����", 1
	);
	acce[3].Init(
		ACCESSORY, "�ݹ���", 1000, 4,
		"������ ������� ����", 1
	);
	acce[4].Init(
		ACCESSORY, "���̾ƹ���", 2000, 5,
		"���̾Ƹ�尡 �����ִ� ����", 1
	);
}

void Shop::ReadItem(ITEM_TYPE category)
{
	switch (category)
	{
	case WEAPON:
		MakeWeapon();
		break;
	case ARMOUR:
		MakeArmour();
		break;
	case POTION:
		MakePotion();
		break;
	case ACCESSORY:
		MakeAcce();
		break;
	}
}

Item* Shop::GetItems(ITEM_TYPE category)
{
	switch (category)
	{
	case WEAPON:
		return weapon;
		break;
	case ARMOUR:
		return armour;
		break;
	case POTION:
		return potion;
		break;
	case ACCESSORY:
		return acce;
		break;
	}
}

int Shop::GetItemIndex(ITEM_TYPE category, string name)
{
	int index;

	switch (category)
	{
	case WEAPON:
		for (int i = 0; i < weaponSize; i++) {
			if (weapon[i].GetName() == name)
				index = i;
		}
		break;
	case ARMOUR:
		for (int i = 0; i < armourSize; i++) {
			if (armour[i].GetName() == name)
				index = i;
		}
		break;
	case POTION:
		for (int i = 0; i < potionSize; i++) {
			if (potion[i].GetName() == name)
				index = i;
		}
		break;
	case ACCESSORY:
		for (int i = 0; i < acceSize; i++) {
			if (acce[i].GetName() == name)
				index = i;
		}
		break;
	}

	return index;
}

int Shop::GetItemCount(ITEM_TYPE category)
{
	int count;

	switch (category)
	{
	case WEAPON:
		count = weaponSize;
			break;
	case ARMOUR:
		count = armourSize;
		break;
	case POTION:
		count = potionSize;
		break;
	case ACCESSORY:
		count = acceSize;
		break;
	}

	return count;
}

void Shop::Init()
{
	ReadItem(WEAPON);
	ReadItem(ARMOUR);
	ReadItem(POTION);
	ReadItem(ACCESSORY);

}

//void Shop::Update()
//{
//}
//
//void Shop::Render()
//{
//}