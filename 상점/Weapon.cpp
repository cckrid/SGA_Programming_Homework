#include "Weapon.h"



Weapon::Weapon()
{
}


Weapon::~Weapon()
{
}

void Weapon::Show()
{
	cout << "�̸�	: " << name << endl;
	cout << "����	: " << price << endl;
	cout << "���ݷ�	: " << stat << endl;
	cout << "����	: " << comment << endl;
	cout << "����	: ";
	if (count == 0)
		cout << "����";
	else
		cout << count;
	cout << endl;
}

void Weapon::InitType()
{
	type = ATK;
}
