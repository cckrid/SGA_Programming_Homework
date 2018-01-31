#include "Shop.h"



Shop::Shop()
{
	Init();
}


Shop::~Shop()
{
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
		for (int i=0; i<weapon.size(); i++){
			weapon[i].MakeLine();
			cout << "������ ��ȣ : " << i + 1 << endl;
			weapon[i].Show();
			weapon[i].MakeLine();
		}
		break;
	case ARMOUR:
		for (int i = 0; i<armour.size(); i++) {
			armour[i].MakeLine();
			cout << "������ ��ȣ : " << i + 1 << endl;
			armour[i].Show();
			armour[i].MakeLine();
		}
		break;
	case POTION:
		for (int i = 0; i<potion.size(); i++) {
			potion[i].MakeLine();
			cout << "������ ��ȣ : " << i + 1 << endl;
			potion[i].Show();
			weapon[i].MakeLine();
		}
		break;
	case ACCESSORY:
		for (int i = 0; i<acce.size(); i++) {
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
	Weapon w[5];
	w[0].Init(
		WEAPON, "������ ��", 500, 4,
		"�� �ٵ���� ����Į. ���԰� �� �������־� �ʺ��ڰ� �ٷ�� ����.", 1
	);
	w[1].Init(
		WEAPON, "�� �ҵ�", 2000, 7,
		"���� �ٷ�� ���� �������� ������ �� �� �ִ� ����", 1
	);
	w[2].Init(
		WEAPON, "�� �ҵ�", 3000, 9,
		"���̰� ��鼭�� �������� ���ݷ��� �ڶ��ϴ� ����.", 1
	);
	w[3].Init(
		WEAPON, "�� �ڵ�� �ҵ�", 7000, 25,
		"���� ����� Ű���� Ŭ ������ ������� �Ѽ����δ� ��� �� �� ����.", 1
	);
	w[4].Init(
		WEAPON, "Ŭ���̸��", 8000, 28,
		"�β��� ��� ź�¼��� �ִ� ������ ���� ���� ���� ���� ������� ��հ�.", 1
	);

	for (int i = 0; i < 5; i++)
		weapon.push_back(w[i]);
}

void Shop::MakeArmour()
{
	Armour a[5];
	a[0].Init(
		ARMOUR, "�ʺ��ڿ� ����", 50, 1,
		"�ʺ��ڰ� ���� �Դ� ������ ���̴�.", 1
	);
	a[1].Init(
		ARMOUR, "��ɲۿ�", 200, 2,
		"���׼����� ���ǿ� ưư�� õ���� �� ������ �̷�����ִ�.", 1
	);
	a[2].Init(
		ARMOUR, "����Ʈ ��������", 600, 4,
		"��¥ ������ ��� ���� ���װ���", 1
	);
	a[3].Init(
		ARMOUR, "ü�θ���", 2000, 8,
		"����� ���� �罽�� ���� �����̴�.", 1
	);
	a[4].Init(
		ARMOUR, "�÷���Ʈ����", 4000, 14,
		"ö���� ����� ���� ưư�� �����̴�.", 1
	);

	for (int i = 0; i < 5; i++)
		armour.push_back(a[i]);
}

void Shop::MakePotion()
{
	string hp[5] = { "10", "30", "50", "100", "300" };
	int price[5] = { 50,200,400,1000,4000 };
	Potion p[5];
	for (int i = 0; i < 5; i++) {
		p[i].Init(
			POTION, "����� " + hp[i] + " ����", price[i], atoi(hp[i].c_str()),
			"������� " + hp[i] + " ȸ���� �� �ִ� ����", 1
		);
		potion.push_back(p[i]);
	}
}

void Shop::MakeAcce()
{
	Accessory a[5];
	a[0].Init(
		ACCESSORY, "�콼 ����", 50, 1,
		"���� ���� ���� �Ҿ���� ����", 1
	);
	a[1].Init(
		ACCESSORY, "����", 100, 2,
		"�����ϰ� ���� ������ ����", 1
	);
	a[2].Init(
		ACCESSORY, "������", 500, 3,
		"������ ������� ����", 1
	);
	a[3].Init(
		ACCESSORY, "�ݹ���", 1000, 4,
		"������ ������� ����", 1
	);
	a[4].Init(
		ACCESSORY, "���̾ƹ���", 2000, 5,
		"���̾Ƹ�尡 �����ִ� ����", 1
	);
	for (int i = 0; i < 5; i++)
		acce.push_back(a[i]);
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

vector<Item>& Shop::GetItems(ITEM_TYPE category)
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
		for (int i = 0; i < weapon.size(); i++) {
			if (weapon[i].GetName() == name)
				index = i;
		}
		break;
	case ARMOUR:
		for (int i = 0; i < armour.size(); i++) {
			if (armour[i].GetName() == name)
				index = i;
		}
		break;
	case POTION:
		for (int i = 0; i < potion.size(); i++) {
			if (potion[i].GetName() == name)
				index = i;
		}
		break;
	case ACCESSORY:
		for (int i = 0; i < acce.size(); i++) {
			if (acce[i].GetName() == name)
				index = i;
		}
		break;
	}

	return index;
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