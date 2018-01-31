#include "Item.h"



Item::Item()
{
}


Item::~Item()
{
}

void Item::Init(ITEM_TYPE itemType, string name, int price, int stat, string comment, int count)
{
	this->itemType = itemType;
	this->name = name;
	this->price = price;
	this->stat = stat;
	this->comment = comment;
	this->count = count;

	InitType();
}

void Item::Show()
{
	cout << "�̸�	: " << name << endl;
	cout << "����	: " << price << endl;
	cout << "�ɷ�ġ	: " << stat << endl;
	cout << "����	: " << comment << endl;
	cout << "����	: " << count << endl;
}

void Item::InitType()
{
	type = NONE;	// �ʱ�ȭ ���� �߻��� ���� �� �־ �ʱ�ȭ
}

void Item::MakeLine()
{
	for (int i = 0; i < MAX_LINE; i++) {
		cout << "=";
	}
	cout << endl;
}

void Item::SetType(STAT_TYPE type)
{
	this->type = type;
}

void Item::SetCount(int count)
{
	this->count = count;
}

string Item::GetName()
{
	return name;
}

int Item::GetPrice()
{
	return price;
}

int Item::GetStat()
{
	return stat;
}

string Item::GetComment()
{
	return comment;
}

int Item::GetCount()
{
	return count;
}

STAT_TYPE Item::GetStatType()
{
	return type;
}

ITEM_TYPE Item::GetItemType()
{
	return itemType;
}
