#pragma once
#include "Item.h"
class Accessory : public Item
{
public:
	Accessory();
	~Accessory();

	void Show() override;

	// Item��(��) ���� ��ӵ�
	virtual void InitType() override;
};

