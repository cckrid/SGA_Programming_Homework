#pragma once
#include "Item.h"
class Armour : public Item
{
public:
	Armour();
	~Armour();

	void Show() override;

	// Item��(��) ���� ��ӵ�
	virtual void InitType() override;
};

