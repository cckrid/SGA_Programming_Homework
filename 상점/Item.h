#pragma once
#include <iostream>
#include <string>

using namespace std;

#define MAX_LINE 50					// �� �߱� ���� ǥ���� ����

enum ITEM_TYPE { WEAPON, ARMOUR, POTION, ACCESSORY }; // ������ ���� ����

enum STAT_TYPE { NONE, HP, MP, ATK, DEF };	// �ɷ�ġ � �κ��� ��½�ų�� ����

class Item
{
protected:
	ITEM_TYPE itemType;
	string name;
	int price;
	STAT_TYPE type;
	int stat;
	string comment;
	int count;

public:
	Item();
	~Item();

	// Item ������� �ʱ�ȭ ��Ű�� �Լ�
	virtual void Init(
		ITEM_TYPE itemType, string name, int price, 
		int stat, string comment, int count);

	// ����ϴ� �ɷ�ġ�� ���� ������(�ɷ�ġ�κи�) �ٸ��� ����ϱ� ����
	// �����Լ��� �����Ͽ� ������ �� �� �ְ���
	virtual void Show();			// Item ������ �����ִ� �Լ�

	// ��ӵ� Ŭ�������� �ٸ��� ������ �ؾ��ϰ� ������ �������ؾ��ؼ�
	// ���� �����Լ��� ����, ��� ���� �̷��� �� �ʿ�� ����
	// ���� �����Լ��� �����Ϸ� ������ �߻� Ŭ������ �ǹ����� 
	// Shop Ŭ�������� Item���� �����ϱ� ���� -> ���ϱ�� ����
	virtual void InitType();	// �ɷ�ġ ��� Ÿ�� ����

	void MakeLine();				// �� �ߴ� �Լ�

	// �ɷ�ġ Ÿ�� ���� �ʿ��� ��츦 ���� �Լ� �߰�
	void SetType(STAT_TYPE type);		
	void SetCount(int count);

	string GetName();
	int GetPrice();
	int GetStat();
	string GetComment();
	int GetCount();
	STAT_TYPE GetStatType();
	ITEM_TYPE GetItemType();
};

