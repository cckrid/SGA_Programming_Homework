#include "Accessory.h"



Accessory::Accessory()
{
}


Accessory::~Accessory()
{
}

void Accessory::Show()
{
	cout << "�̸�	: " << name << endl;
	cout << "����	: " << price << endl;
	cout << "����	: " << stat << endl;
	cout << "����	: " << comment << endl;
	cout << "����	: ";
	if (count == 0)
		cout << "����";
	else
		cout << count;
	cout << endl;
}

void Accessory::InitType()
{
	type = MP;
}
