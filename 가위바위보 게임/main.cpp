// 1.15(��) Day 3
// ���� : �ݺ����� ���ǹ�, rand()�� �̿��� ���� ���� �� ����

// ���� ���
// �÷��̾� 2�� 1���� ��, �Ѹ��� ��ǻ��
// ����(0), ����(1), ��(2)
// ���� �ϳ� ����
// �� 5�� �÷���
// �� �÷��̸��� ���� ��� �̰��. ��� ���� ���
// ex) ����ڰ� ������ ��ǻ�͸� �̰��
// ���������� �̱� Ƚ��, �� Ƚ��, ��� Ƚ�� ���
// ī�信 ������ �ø���

#include <iostream>
#include <time.h>
using namespace std;

void makeCover(int star) {
	for (int i = 0; i < star; i++) {
		cout << "*";
	}
	cout << endl;
}

void main()
{
	int star = 49;
	bool isSelect = false;
	int player;

	makeCover(star);

	cout << "*\t\t���� ���� �� ����\t\t*" << endl;

	makeCover(star);

	while (isSelect == false) {
		cout << "���� ���� �� �� �ϳ��� �Է��ϼ���" << endl;
		cout << "����Ű ����(0), ����(1), ��(2)" << endl;
		cout << "�Է� : ";
		
		cin >> player;
		cout << endl;

		switch (player) {
		case 0:
			cout << "[����(0)]�� �Է� �ϼ̽��ϴ�." << endl;
			isSelect = true;
			break;
		case 1:
			cout << "[����(1)]�� �Է� �ϼ̽��ϴ�." << endl;
			isSelect = true;
			break;
		case 2:
			cout << "[��(2)]�� �Է� �ϼ̽��ϴ�." << endl;
			isSelect = true;
			break;
		default:
			cout << "�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ���" << endl;
			cout << "�Է��Ͻ� ���ڴ� " << player << "�Դϴ�." << endl;
			break;
		}
	}

	cout << endl;

	makeCover(star);
	
	cout << "*\t\t������ �����մϴ�.\t\t*" << endl;

	makeCover(star);

	srand(time(NULL));

	// �÷��� Ƚ��
	int num = 5;
	int win, lose, draw;
	win = lose = draw = 0;
	for (int i = 0; i < num; i++) {
		cout << i + 1 << "��° ����" << endl;
		
		// ��ǻ�� ����
		int com = rand() % 3;
		cout << "\t��ǻ�ʹ� ";
		
		switch (com) {
		case 0:
			cout << "����(0)�� �½��ϴ�." << endl;
			break;
		case 1:
			cout << "����(1)�� �½��ϴ�." << endl;
			break;
		case 2:
			cout << "��(2)�� �½��ϴ�." << endl;
			break;
		}

		cout << "\t�����... ";

		// draw
		if (com == player) {
			cout << "�����ϴ�." << endl;
			draw++;
		}
		// player win
		// �� ��(2), �÷��̾� ����(2)�� �� �� ���� Ŭ �� �̱�
		else if (com < player || (com == 2 && player == 0)) {
			cout << "�̰���ϴ�." << endl;
			win++;
		}
		// lose
		else {
			cout << "�����ϴ�." << endl;
			lose++;
		}
	}

	makeCover(star);
	cout << "*\t\t���� ���\t\t\t*" << endl;
	cout << "*\t\t" << win << "��, " << lose << "��, " << draw << "��" << "\t\t\t*" << endl;
	makeCover(star);
}