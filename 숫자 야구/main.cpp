// 1.16(ȭ) Day 4
// ���� : ���� �߱� ���� �����
// �ߺ����� �ʴ� ����

#include <iostream>
#include <time.h>

using namespace std;

void main() {
	srand(time(NULL));

	int GameCount = 0;

	int number[10];
	int baseBallNumber[4];
	int selectNumber[4];

	for (int i = 0; i < 10; i++)
		number[i] = i ;

	// ����
	for (int i = 0; i < 1000; i++)
	{
		int des, src, temp;
		des = rand() % 10;
		src = rand() % 10;

		temp = number[des];
		number[des] = number[src];
		number[src] = temp;
	}

	// �׽�Ʈ
	//for (int i = 0; i < 10; i++)
	//	cout << number[i] << " ";
	//cout << endl;

	// ���õ� ���� �� 4���� �߱� ���ڷ� ����
	for (int i = 0; i < 4; i++)
		baseBallNumber[i] = number[i];

	while (true) {

		cout << GameCount << "��° ����" << endl;
		
		// ġƮ
		cout << "[ġƮ] ���� : ";
		for (int i = 0; i < 4; i++)
			cout << baseBallNumber[i] << " ";
		cout << endl;

		int strike, ball;
		strike = ball = 0;

		bool isCheck = false;
		cout << "\t�ߺ����� �ʴ� 4���� ���ڸ� �Է��ϼ��� : ";

		// 4���� �Է°� �ޱ�
		for (int i = 0; i < 4; i++) {
			cin >> selectNumber[i];
		}

		// �Է°� ���� ó��
		// �߸� �Էµ� ����
		for (int i = 0; i < 4; i++) {
			if (selectNumber[i] < 0 || selectNumber[i] > 9) {
				cout << "\t�� �� �Է��Ͽ����ϴ�. �ٽ� �Է��ϼ���" << endl;
				isCheck = true;
				break;
			}
		}
		if (isCheck == false) {
			// �ߺ��� ���� ó��
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					if (j == k)
						continue;
					else if (selectNumber[j] == selectNumber[k]) {
						cout << "\t�ߺ��� ���� �ֽ��ϴ�. �ٽ� �Է��ϼ���." << endl;
						isCheck = true;
						break;
					}
				}
				if (isCheck == true)
					break;
			}
		}

		if (isCheck == true) {
			continue;
		}

		// ������ ���� ��
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (baseBallNumber[i] == selectNumber[j]) {
					if (i == j)
						strike++;
					else
						ball++;
				}
			}
		}

		cout << "\t���� ��� : " << "��Ʈ����ũ " << strike << ","
			<< "�� " << ball << ", " << "�ƿ� " << 4 - strike - ball << endl;
		
		if (strike == 4) {
			cout << "\t���ӿ��� �¸��Ͽ����ϴ�." << endl;
			break;
		}

		GameCount++;
		cout << endl;

		// 10�� �̻�� �й�
		if (GameCount > 10) {
			cout << "�й� �Ͽ����ϴ�." << endl;
			break;
		}

	}
}

// int �� char �� cin���� ������ �ȵ��� �� ���� 
// �ذ� ��� ã�ƺ���