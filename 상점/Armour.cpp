#include "Armour.h"



Armour::Armour()
{
}


Armour::~Armour()
{
}

void Armour::Show()
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

void Armour::InitType()
{
	type = DEF;
}
