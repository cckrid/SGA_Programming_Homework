#include "Potion.h"



Potion::Potion()
{
}


Potion::~Potion()
{
}

void Potion::Show()
{
	cout << "�̸�	: " << name << endl;
	cout << "����	: " << price << endl;
	cout << "HPȸ��	: " << stat << endl;
	cout << "����	: " << comment << endl;
	cout << "����	: ";
	if (count == 0)
		cout << "����";
	else
		cout << count;
	cout << endl;
}

void Potion::InitType()
{
	type = HP;
}
