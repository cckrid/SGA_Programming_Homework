// Ư������ ���� ���ڿ� ������
// "abs/sae&stge[" 

#include <iostream>

using namespace std;

bool isAlphabet(char c) {
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return true;
	else
		return false;
}

void main() {
	char str[100];

	cout << "���ڸ� �Է��ϼ���. (100�� �̳�)" << endl;
	cout << "�Է� : ";
	cin >> str;

	cout << "�Է��� ���ڿ� : " << str << endl;

	int len = strlen(str);
	int start = 0;
	int end = len - 1;

	while(start < end) {
		// check
		cout << "\tstart: " << str[start] << ", end: " << str[end] << endl;

		// alphabet �� ��� swap
		if (isAlphabet(str[start]) && isAlphabet(str[end])) {
			cout << "\t\t�� �� ���ĺ� swap" << endl;
			char temp;
			temp = str[start];
			str[start] = str[end];
			str[end] = temp;
			start++;
			end--;
		}
		// start�� ���ĺ��� �ƴ� ���
		else if (isAlphabet(str[start]) == false) {
			cout << "\t\tstart ���� ���ڷ�" << endl;
			start++;
		}
		// end�� ���ĺ��� �ƴ� ���
		else {
			cout << "\t\tend ���� ���ڷ�" << endl;
			end--;
		}
	}

	cout << "������ ���ڿ� : " << str << endl;
	
}