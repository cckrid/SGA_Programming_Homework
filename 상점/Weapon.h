#pragma once
#include "Item.h"
class Weapon : public Item
{
public:
	Weapon();
	~Weapon();

	void Show() override;

	// Item��(��) ���� ��ӵ�
	virtual void InitType() override;
};

