#pragma once
#include "Item.h"
class Potion : public Item
{
public:
	Potion();
	~Potion();

	void Show() override;

	// Item��(��) ���� ��ӵ�
	virtual void InitType() override;
};

