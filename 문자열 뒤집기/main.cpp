#include <iostream>
#include <time.h>

using namespace std;

void main() {
	char str[100];

	cout << "���ڸ� �Է��ϼ���. (100�� �̳�)" << endl;
	cout << "�Է� : ";
	cin >> str;

	cout << "�Է��� ���ڿ� : " << str << endl;

	int len = strlen(str);

	for (int i = 0; i < len / 2; i++) {
		char temp;

		temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}

	cout << "������ ���ڿ� : " << str << endl;

}